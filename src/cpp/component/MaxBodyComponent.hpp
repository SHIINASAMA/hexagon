#pragma once

#include "IComponent.hpp"

class MaxBodyComponent final : public IFilterComponent {
public:
    MaxBodyComponent() = default;

    void setSize(uint32_t size);

    sese::service::http::HttpService::FilterCallback getFilter() override;

private:
    uint32_t size{};
};
