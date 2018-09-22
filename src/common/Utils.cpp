#include "Utils.h"

#include <iostream>
#include <sstream>
#include <fstream>

void fail(boost::system::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}

std::string readFile(const std::string &name) {
    std::ifstream input(name);
    std::stringstream sstr;

    while(input >> sstr.rdbuf());
    return sstr.str();
}
