#include "RootCertificates.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using tcp = boost::asio::ip::tcp;
namespace po = boost::program_options;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

struct Endpoint {
    std::string host;
    std::string port;
    std::string path;

    Endpoint(const std::string & s)
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
};

std::ostream & operator<<(std::ostream & os, Endpoint const & v) {
    os << v.host << ":" << v.port << v.path;
    return os;
}

struct ClientArguments {
    Endpoint endpoint;
    bool quiet;

    ClientArguments(const Endpoint & endpoint, bool quiet)
        : endpoint(endpoint)
        , quiet(quiet)
    {}
};

ClientArguments parseCommandLine(int argc, char** argv) {
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

int main(int argc, char** argv)
{
    try
    {
        const auto clientArguments = parseCommandLine(argc, argv);
        auto const text = "hello world";

        if (!clientArguments.quiet) {
            std::cout << "connecting to endpoint " << clientArguments.endpoint << std::endl;
        }

        boost::asio::io_context ioc;

        ssl::context ctx{ssl::context::sslv23_client};
        load_root_certificates(ctx);

        tcp::resolver resolver{ioc};
        websocket::stream<ssl::stream<tcp::socket>> ws{ioc, ctx};

        auto const results = resolver.resolve(clientArguments.endpoint.host, clientArguments.endpoint.port);
        // TODO error handling
        boost::asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());
        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(clientArguments.endpoint.host, clientArguments.endpoint.path);

        // Send the message
        ws.write(boost::asio::buffer(std::string(text)));

        // This buffer will hold the incoming message
        boost::beast::multi_buffer b;

        // Read a message into our buffer
        ws.read(b);

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The buffers() function helps print a ConstBufferSequence
        std::cout << boost::beast::buffers(b.data()) << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
