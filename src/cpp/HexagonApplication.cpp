#include "HexagonApplication.hpp"

#include <filesystem>

#include <sese/security/SSLContextBuilder.h>
#include <sese/io/File.h>
#include <sese/util/Exception.h>
#include <sese/Log.h>

HexagonApplication::HexagonApplication(const std::string &base_path) {
    auto config_path = base_path + "/config.yml";
    auto file = sese::io::File::create(config_path, "r");
    if (!file) {
        throw sese::Exception("failed to open config.yml");
    }
    auto config = HexagonConfig(file.get());

    server.setName(config.name);
    server.setKeepalive(config.keepalive);

    for (auto &&[uri_prefix, path]: config.mounts) {
        auto local = base_path + "/" + path;
        server.regMountPoint(uri_prefix, local);
        SESE_INFO("Mounting {} -> {}", uri_prefix, local);
    }

    for (auto &&[ip, port, cert, pkey]: config.services) {
        auto ipaddr = sese::net::IPAddress::create(ip.c_str(), port);
        std::unique_ptr<sese::security::SSLContext> context;

        if (!cert.empty() && !pkey.empty()) {
            cert = base_path + "/" + cert;
            pkey = base_path + "/" + pkey;
            context = sese::security::SSLContextBuilder::UniqueSSL4Server();
            if (!context->importCertFile(cert.c_str())) {
                throw sese::Exception("failed to import certificate file");
            }
            if (!context->importPrivateKeyFile(pkey.c_str())) {
                throw sese::Exception("failed to import private key file");
            }
            if (!context->authPrivateKey()) {
                throw sese::Exception("failed to auth private key");
            }
        }
        server.regService(ipaddr, std::move(context));
    }

    max_body_component.setSize(config.max_body_size);
    server.regFilter("/", max_body_component.getFilter());

    for (auto &&[k, v]: config.mappings) {
        SESE_INFO("Mapping {} -> {}", k, v);
        mapping_component.set(k, v);
    }
    server.regFilter("/", mapping_component.getFilter());

    for (auto &&[k, v]: config.friendly_response) {
        auto path = base_path + "/" + v;
        SESE_INFO("Friendly response {} -> {}", k, path);
        friendly_response_component.set(k, path);
    }
    server.regTailFilter(friendly_response_component.getFilter());
}

void HexagonApplication::startup() const {
    auto result = server.startup();
    if (!result) {
        throw sese::Exception("failed to start server");
    }
}

void HexagonApplication::shutdown() const {
    server.shutdown(); // NOLINT
}
