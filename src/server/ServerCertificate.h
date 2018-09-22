#pragma once

#include <boost/asio/ssl/context.hpp>

#include "Arguments.h"

#include <cstddef>
#include <memory>

namespace server {

void loadServerCertificates(boost::asio::ssl::context& ctx, const SSLArguments & args);

}
