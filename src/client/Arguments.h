#pragma once

#include <iostream>
#include <vector>

namespace networkstream { namespace client {

struct Endpoint {
    std::string host;
    std::string port;
    std::string path;

    Endpoint(const std::string & s);
};

std::ostream & operator<<(std::ostream & os, Endpoint const & v);

struct Arguments {
    Endpoint endpoint;
    bool quiet;

    Arguments(const Endpoint & endpoint, bool quiet)
        : endpoint(endpoint)
        , quiet(quiet)
    {}
};

Arguments parseCommandLine(int argc, char** argv);

}}
