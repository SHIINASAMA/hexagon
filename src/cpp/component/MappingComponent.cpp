#include "MappingComponent.hpp"

void MappingComponent::set(const std::string &src, const std::string &dst) {
    mappings[src] = dst;
}

sese::service::http::HttpService::FilterCallback MappingComponent::getFilter() {
    return [this](sese::net::http::Request &req, sese::net::http::Response &resp)-> bool {
        auto iterator = this->mappings.find(req.getUri());
        if (iterator != this->mappings.end()) {
            resp.setCode(301);
            resp.set("location", iterator->second);
            return false;
        }
        return true;
    };
}
