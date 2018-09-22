#include "Server.h"

#include <iostream>

using namespace networkstream::server;

int main(int argc, char** argv) {
    try {
        run(parseCommandLine(argc, argv));
    } catch(std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
