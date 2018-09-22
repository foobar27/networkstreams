#include "Client.h"

using namespace networkstream::client;

int main(int argc, char** argv) {
    try {
        const auto args = parseCommandLine(argc, argv);
        auto const text = "hello world";
        run(args, text);
    } catch(std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
