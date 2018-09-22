#include "Arguments.h"

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <exception>
#include <filesystem>

#include "../common/Utils.h"

namespace server {

namespace fs = std::filesystem;

fs::path combinePaths(const fs::path & dir, const std::string & file) {
    auto combined = dir /  file;
    if (!fs::exists(combined)) {
        throw std::runtime_error(std::string("File not found: ") + combined.generic_string());
    }
    return combined;
}

void fillConfigFile(Arguments & args, const fs::path & configFile) {
    namespace pt = boost::property_tree;

    if (!fs::exists(configFile)) {
        throw std::runtime_error(std::string("Config file not found: ") + configFile.generic_string());
    }

    pt::ptree tree;
    pt::read_json(configFile, tree);

    auto configPath = configFile.parent_path();
    if (!fs::exists(configPath) || !fs::is_directory(configPath)) {
        throw std::runtime_error(std::string("Config path not found: ") + configPath.generic_string());
    }

    auto certFile = combinePaths(configPath, tree.get<std::string>("ssl.certificate", "keys/cert.pem"));
    auto keyFile = combinePaths(configPath, tree.get<std::string>("ssl.key", "keys/key.pem"));
    auto dhFile = combinePaths(configPath, tree.get<std::string>("ssl.dh", "keys/dh.pem"));

    args.ssl.sslCertificate = readFile(certFile);
    args.ssl.sslKey = readFile(keyFile);
    args.ssl.sslDh = readFile(dhFile);
}

Arguments parseCommandLine(int argc, char** argv) {
    using namespace std;
    namespace po = boost::program_options;

    Arguments serverArguments;
    po::options_description desc("Allowed options");
    std::string address_string;
    std::string configFile;
    desc.add_options()
        ("help,h", "produce help message")
        ("config,c", po::value(&configFile)->default_value("/etc/network-stream/server/config.json"))
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

    fillConfigFile(serverArguments, fs::path(configFile));

    serverArguments.address = boost::asio::ip::make_address(address_string);
    return serverArguments;
}

}
