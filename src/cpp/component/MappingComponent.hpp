#pragma once

#include "IComponent.hpp"

class MappingComponent : IFilterComponent {
public:
    MappingComponent() =  default;

    sese::service::http::HttpService::FilterCallback getFilter() override;

private:
    std::map<std::string, std::string> mappings;
};
