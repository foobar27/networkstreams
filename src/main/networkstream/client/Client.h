#pragma once

#include "Arguments.h"

#include <boost/asio/ssl.hpp>

namespace networkstream { namespace client {

class Client {
    Arguments m_args;
    boost::asio::ssl::context m_ctx {boost::asio::ssl::context::sslv23_client};
public:
    Client(const Arguments & args)
        : m_args(args)
    {}
    void addRootCertificate(const std::string &);
    std::string execute(const std::string & request);
};

}}
