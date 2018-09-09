#include "Arguments.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace server {

namespace po = boost::program_options;

Arguments parseCommandLine(int argc, char** argv) {
    using namespace std;
    Arguments serverArguments;
    po::options_description desc("Allowed options");
    std::string address_string;
    desc.add_options()
        ("help", "produce help message")
        ("address,a", po::value(&address_string)->default_value("0.0.0.0"))
        ("port,p", po::value(&serverArguments.port)->default_value(443), "the port where the server is listening")
        ("threads", po::value(&serverArguments.threads)->default_value(1))
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(1);
    }

    if (serverArguments.threads < 1) {
        std::cerr << "threads must be at least 1." << std::endl;
        exit(1);
    }

    serverArguments.address = boost::asio::ip::make_address(address_string);
    return serverArguments;
}

}
