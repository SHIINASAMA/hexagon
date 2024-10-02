#pragma once

#include <string>

#include <sese/service/http/HttpServer.h>

#include "HexagonConfig.hpp"

class HexagonApplication {
public:
    explicit HexagonApplication(const std::string &base_path);

    void startup() const;

    void shutdown() const;

    void customization();

private:
    sese::service::http::HttpServer server;
};
