#include "Utils.h"

#include <iostream>
#include <sstream>
#include <fstream>

void fail(boost::system::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

std::string readFile(const std::filesystem::path & path) {
    using namespace std;
    using namespace filesystem;
    std::ifstream f{ path };

    const auto sz = file_size(path);
    std::string result(sz, '\0');
    f.read(result.data(), sz);
    return result;
}
