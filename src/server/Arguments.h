 #pragma once

#include <boost/asio/ip/tcp.hpp>

namespace server {

struct Arguments {
    boost::asio::ip::address address;
    unsigned short port;
    int threads;
};

Arguments parseCommandLine(int argc, char** argv);
}
