#include "FriendlyResponseComponent.hpp"

#include <sese/io/File.h>

#include <filesystem>

sese::service::http::HttpService::FilterCallback FriendlyResponseComponent::getFilter() {
    return [this](sese::net::http::Request &req, sese::net::http::Response &resp)-> bool {
        auto iterator = this->file_map.find(resp.getCode());
        if (iterator != this->file_map.end()) {
            auto file = sese::io::File::create(iterator->second, "r");
            sese::streamMove(&resp.getBody(), file.get(), std::filesystem::file_size(iterator->second));
            return true;
        }
        return false;
    };
}

void FriendlyResponseComponent::set(int code, const std::string &file) {
    file_map[code] = file;
}
