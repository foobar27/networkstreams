#pragma once

#include <boost/system/error_code.hpp>
#include <filesystem>

namespace networkstream {

void fail(boost::system::error_code ec, char const* what);

std::string readFile(const std::filesystem::path & name);

}
