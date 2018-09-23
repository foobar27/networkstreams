#pragma once

#include <iostream>
#include <vector>

namespace networkstream { namespace client {

struct Endpoint {
    std::string host;
    std::string port;
    std::string path;

    Endpoint(const std::string & host, const std::string & port, const std::string & path)
        : host(host)
        , port(port)
        , path(path)
    {}

    Endpoint(const std::string & s);
};

std::ostream & operator<<(std::ostream & os, Endpoint const & v);

struct Arguments {
    Endpoint endpoint;
};

Arguments parseCommandLine(int argc, char** argv);

}}
