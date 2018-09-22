#pragma once

#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>
#include <cstddef>
#include <memory>

void loadServerCertificate(boost::asio::ssl::context& ctx);

