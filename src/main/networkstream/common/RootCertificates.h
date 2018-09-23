#pragma once

#include <boost/asio/ssl.hpp>

namespace networkstream {

void addRootCertificate(boost::asio::ssl::context& ctx, const std::string & certificate, boost::system::error_code& ec);

void addRootCertificate(boost::asio::ssl::context& ctx, const std::string & certificate);

}
