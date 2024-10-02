#include "HexagonApplication.hpp"

#include <filesystem>

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

    server.regFilter("/", [](sese::net::http::Request &req, sese::net::http::Response &resp) {
        if (req.getUri() == "/") {
            resp.setCode(301);
            resp.set("location", "/index.html");
            return false;
        }
        return true;
    });

    server.regTailFilter([](sese::net::http::Request &req, sese::net::http::Response &resp) {
        if (resp.getCode() == 404) {
            auto str = "D:/workspaces/hexagon/out/404.html";
            auto file = sese::io::File::create(str, "r");
            sese::streamMove(&resp.getBody(), file.get(), std::filesystem::file_size(str));
            return true;
        }
        return false;
    });
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
