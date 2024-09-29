#pragma once

#include <vector>
#include <string>
#include <map>

#include <sese/io/InputStream.h>
#include <sese/util/Value.h>

struct HexagonConfig {
    struct Service {
        std::string ip;
        uint32_t port;
        std::string cert;
        std::string pkey;
        uint32_t threads = 1;
    };

    std::string name;
    uint32_t keepalive = 30;
    std::vector<Service> services;
    std::map<std::string, std::string> mounts;

    explicit HexagonConfig(sese::io::InputStream *input);

    void parseServer(sese::Value *value);
    void parseService(sese::Value *value);
    void parseMount(sese::Value *value);
};