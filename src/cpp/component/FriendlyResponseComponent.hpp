#pragma once

#include "IComponent.hpp"

class FriendlyResponseComponent final : public IFilterComponent {
public:
    sese::service::http::HttpService::FilterCallback getFilter() override;

    void set(int code, const std::string &file);

private:
    std::map<int, std::string> file_map;
};
