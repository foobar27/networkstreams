#include "Arguments.h"

#include <string>
#include <vector>

#include "boost/program_options.hpp"\

namespace client {

namespace po = boost::program_options;

Endpoint::Endpoint(const std::string & s)
    : port("443")
    , path("/"){
    using namespace std;
    auto colon = s.find(":");
    if (colon != string::npos) {
        this->host = s.substr(0, colon);
        auto slash = s.find("/");
        this->port = s.substr(colon + 1, slash - colon - 1);
        if (slash != string::npos) {
            this->path = s.substr(slash);
        }
    } else {
        // no port given
        auto slash = s.find("/");
        this->host = s.substr(0, slash);
        if (slash != std::string::npos) {
            this->path = s.substr(slash);
        }
    }
}

std::ostream & operator<<(std::ostream & os, Endpoint const & v) {
    os << v.host << ":" << v.port << v.path;
    return os;
}

Arguments parseCommandLine(int argc, char** argv) {
    using namespace std;
    po::options_description desc("Allowed options");
    string endpoint;
    desc.add_options()
        ("help", "produce help message")
        ("endpoint", po::value(&endpoint)->required(), "the endpoint, e.g. localhost:443/path (default port: 443, default path: /)")
        ("quiet,q", "Disable logging")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(1);
    }
    bool quiet = vm.count("quiet") > 0;
    return {{endpoint}, quiet};
}

}
