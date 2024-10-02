#include "HexagonApplication.hpp"

#include <sese/io/File.h>
#include <sese/util/Exception.h>
#include <sese/Log.h>

HexagonApplication::HexagonApplication(const std::string &base_path) {
    auto file = sese::io::File::create(base_path + "/config.yml", "r");
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

    for (auto &&[ip, port, cert, pkey, threads]: config.services) {
        auto ipaddr = sese::net::IPAddress::create(ip.c_str(), port);
        for (auto i = 0; i < threads; ++i) {
            server.regService(ipaddr, nullptr);
        }
    }
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

