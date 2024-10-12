#pragma once

#include <string>

#include <sese/service/http/HttpServer.h>

#include "HexagonConfig.hpp"
#include "component/MappingComponent.hpp"
#include "component/FriendlyResponseComponent.hpp"
#include "component/MaxBodyComponent.hpp"

class HexagonApplication {
public:
    explicit HexagonApplication(const std::string &base_path);

    void startup() const;

    void shutdown() const;

    void customization();
private:
    sese::service::http::HttpServer server;

    MappingComponent mapping_component;
    FriendlyResponseComponent friendly_response_component;
    MaxBodyComponent max_body_component;
};
