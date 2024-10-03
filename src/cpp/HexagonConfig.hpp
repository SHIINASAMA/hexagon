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
    };

    std::string name;
    uint32_t keepalive = 30;
    std::vector<Service> services;
    std::map<std::string, std::string> mounts;
    std::map<std::string, std::string> mappings;
    std::map<int, std::string> friendly_response;

    /// \brief 读取配置文件
    ///
    /// 读取给定的配置文件流,并将其解析成一个配置对象
    ///
    /// \param input 配置文件流
    explicit HexagonConfig(sese::io::InputStream *input);

    void parseServer(sese::Value *value);

    void parseService(sese::Value *value);

    void parseMount(sese::Value *value);

    void parseFriendlyResponse(sese::Value *value);

    void parseMappings(sese::Value *value);
};
