#pragma once

#include <boost/asio/ssl.hpp>

namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>

void loadRootCertificates(ssl::context& ctx, boost::system::error_code& ec);

void loadRootCertificates(ssl::context& ctx);
