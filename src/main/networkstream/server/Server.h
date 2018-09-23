#pragma once

#include "Arguments.h"
#include <future>

#include <boost/asio/ssl.hpp>

namespace networkstream { namespace server {

class Server {
    boost::asio::io_context m_ioc;
    Arguments m_args;
    std::vector<std::thread> m_threads;
    boost::asio::ssl::context m_ctx {boost::asio::ssl::context::sslv23};

    void awaitTerminated();
public:
    Server(const Arguments & args);
    ~Server();
    void start();
    void stop();
};

void run(const Arguments & args);

}}
