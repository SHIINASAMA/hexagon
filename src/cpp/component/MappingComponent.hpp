#pragma once

#include "IComponent.hpp"

class MappingComponent final : public IFilterComponent {
public:
    MappingComponent() =  default;

    void set(const std::string &src, const std::string &dst);

    sese::service::http::HttpService::FilterCallback getFilter() override;

private:
    std::map<std::string, std::string> mappings;
};
