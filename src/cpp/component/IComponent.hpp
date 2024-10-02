#pragma once

#include <sese/net/http/Controller.h>
#include <sese/service/http/HttpService.h>

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
