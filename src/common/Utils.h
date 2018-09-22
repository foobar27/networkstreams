#pragma once

#include <boost/system/error_code.hpp>

void fail(boost::system::error_code ec, char const* what);

std::string readFile(const std::string &name);
