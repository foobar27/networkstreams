#pragma once

#include <boost/asio/ip/tcp.hpp>

namespace server {

struct SSLArguments {
    std::string sslCertificate;
    std::string sslKey;
    std::string sslDh;
};

struct Arguments {
    boost::asio::ip::address address;
    unsigned short port;
    int threads;
    SSLArguments ssl;
};

Arguments parseCommandLine(int argc, char** argv);
}
