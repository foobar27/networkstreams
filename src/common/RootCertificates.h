#pragma once

#include <boost/asio/ssl.hpp>

namespace ssl = boost::asio::ssl; // from <boost/asio/ssl.hpp>

void load_root_certificates(ssl::context& ctx, boost::system::error_code& ec);

void load_root_certificates(ssl::context& ctx);
