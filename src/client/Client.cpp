#include "../common/RootCertificates.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>

#include "Arguments.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;



int main(int argc, char** argv)
{
    try
    {
        const auto clientArguments = client::parseCommandLine(argc, argv);
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
