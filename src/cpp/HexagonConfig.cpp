#include "HexagonConfig.hpp"

#include <sese/Log.h>
#include <sese/config/Yaml.h>
#include <sese/util/Exception.h>

HexagonConfig::HexagonConfig(sese::io::InputStream *input) {
    auto value = sese::Yaml::parse(input, 6);
    parseServer(&value);
}

void HexagonConfig::parseServer(sese::Value *value) {
    if (!value->isDict()) {
        throw sese::Exception("top elements is not a dict type");
    }

    auto server = value->getDict().find("server");
    if (server == nullptr ||
        !server->isDict()) {
        throw sese::Exception("element must be dict type");
    }

    auto name = server->getDict().find("name");
    if (name == nullptr ||
        !name->isString()) {
        this->name = "http_server";
    } else {
        this->name = name->getString();
    }

    auto keepalive = server->getDict().find("keepalive");
    if (keepalive == nullptr ||
        !keepalive->isInt()) {
        this->keepalive = 30;
    } else {
        this->keepalive = static_cast<uint32_t>(keepalive->getInt());
    }

    parseService(server->getDict().find("services"));
    parseMount(server->getDict().find("mounts"));
    parseMappings(server->getDict().find("mappings"));
    parseFriendlyResponse(server->getDict().find("friendly_responses"));
}

void HexagonConfig::parseService(sese::Value *value) {
    if (value == nullptr ||
        !value->isList()) {
        throw sese::Exception("services must be dict type");
    }

    for (auto &&item: value->getList()) {
        if (!item.isDict()) {
            throw sese::Exception("service's element must be dict type");
        }
        Service service;
        auto threads = item.getDict().find("threads");
        if (threads == nullptr ||
            !threads->isInt()) {
            service.threads = 2;
        } else {
            service.threads = static_cast<uint32_t>(threads->getInt());
        }
        auto ip = item.getDict().find("ip");
        if (ip == nullptr ||
            !ip->isString()) {
            service.ip = "127.0.0.1";
        } else {
            service.ip = ip->getString();
        }
        auto port = item.getDict().find("port");
        if (port == nullptr ||
            !port->isInt()) {
            throw sese::Exception("port must be int type");
        } else {
            service.port = static_cast<uint32_t>(port->getInt());
        }

        auto cert = item.getDict().find("cert-file");
        if (cert == nullptr ||
            !cert->isString()) {
            service.cert = "";
        } else {
            service.cert = cert->getString();
        }

        auto pkey = item.getDict().find("pkey-file");
        if (pkey == nullptr ||
            !pkey->isString()) {
            service.pkey = "";
        } else {
            service.pkey = pkey->getString();
        }

        services.emplace_back(service);
    }
}

void HexagonConfig::parseMount(sese::Value *value) {
    if (value == nullptr ||
        !value->isList()) {
        throw sese::Exception("mounts must be list type");
    }
    for (auto &item: value->getList()) {
        if (!item.isDict()) {
            throw sese::Exception("mount's element must be object type");
        }
        auto v = item.getDict().begin()->second;
        if (!v->isString()) {
            throw sese::Exception("mount's value must be string type");
        }
        mounts[item.getDict().begin()->first] = v->getString();
    }
}

void HexagonConfig::parseFriendlyResponse(sese::Value *value) {
    if (value == nullptr ||
        !value->isList()) {
        throw sese::Exception("friendly response must be list type");
    }
    for (auto &item: value->getList()) {
        if (!item.isDict()) {
            throw sese::Exception("friendly response's element must be object type");
        }
        char *end;
        auto k = std::strtol(item.getDict().begin()->first.c_str(), &end, 10);
        if (*end != 0) {
            throw sese::Exception("friendly response's key must be integer");
        }
        auto v = item.getDict().begin()->second;
        if (!v->isString()) {
            throw sese::Exception("friendly response's value must be string type");
        }
        friendly_response[k] = v->getString();
    }
}

void HexagonConfig::parseMappings(sese::Value *value) {
    if (value == nullptr ||
        !value->isList()) {
        throw sese::Exception("mappings must be list type");
        }
    for (auto &item: value->getList()) {
        if (!item.isDict()) {
            throw sese::Exception("mapping's element must be object type");
        }
        auto v = item.getDict().begin()->second;
        if (!v->isString()) {
            throw sese::Exception("mapping's value must be string type");
        }
        mappings[item.getDict().begin()->first] = v->getString();
    }
}

