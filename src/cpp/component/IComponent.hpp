#pragma once

#include "../HexagonApplication.hpp"

class IServletComponent {
public:
    virtual ~IServletComponent() = default;

    virtual sese::net::http::Servlet &getServlet() = 0;
};

class IFilterComponent {
public:
    virtual ~IFilterComponent() = default;

    virtual sese::service::http::HttpService::FilterCallback getFilter() = 0;
};
