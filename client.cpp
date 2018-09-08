#include "root_certificates.h"

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

struct ClientArguments {
    std::string host;
    std::string port;
    std::string path {"/"};
};

ClientArguments parseCommandLine(int argc, char** argv) {
    using namespace std;
    po::options_description desc("Allowed options");
    ClientArguments clientArguments;
    desc.add_options()
        ("help", "produce help message")
        ("host,h", po::value(&clientArguments.host)->required(), "the IP or DNS name of the server to connect")
        ("port,p", po::value(&clientArguments.port)->default_value("443"), "the port on the server to connect to")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(1);
    }
    return clientArguments;
}

int main(int argc, char** argv)
{
    try
    {
        const auto clientArguments = parseCommandLine(argc, argv);
        auto const text = "hello world";

        boost::asio::io_context ioc;

        ssl::context ctx{ssl::context::sslv23_client};
        load_root_certificates(ctx);

        tcp::resolver resolver{ioc};
        websocket::stream<ssl::stream<tcp::socket>> ws{ioc, ctx};

        auto const results = resolver.resolve(clientArguments.host, clientArguments.port);
        // TODO error handling
        boost::asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());
        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(clientArguments.host, clientArguments.path);

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
