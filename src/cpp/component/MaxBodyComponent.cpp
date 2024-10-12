#include "MaxBodyComponent.hpp"

void MaxBodyComponent::setSize(uint32_t size) {
    this->size = size;
}

sese::service::http::HttpService::FilterCallback MaxBodyComponent::getFilter() {
    return [this](sese::net::http::Request &req, sese::net::http::Response &resp)-> bool {
        if (sese::toInteger(req.get("content-length", "0")) > this->size) {
            resp.setCode(413);
            resp.set("x-content-limit", std::to_string(this->size) + "bytes");
            return false;
        }
        return true;
    };
}
