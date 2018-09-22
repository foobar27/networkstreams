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

namespace networkstream { namespace client {

void run(const Arguments & args, const std::string & text) {
    if (!args.quiet) {
        std::cout << "connecting to endpoint " << args.endpoint << std::endl;
    }

    boost::asio::io_context ioc;

    ssl::context ctx{ssl::context::sslv23_client};
    loadRootCertificates(ctx);

    tcp::resolver resolver{ioc};
    websocket::stream<ssl::stream<tcp::socket>> ws{ioc, ctx};

    auto const results = resolver.resolve(args.endpoint.host, args.endpoint.port);
    // TODO error handling
    boost::asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());
    ws.next_layer().handshake(ssl::stream_base::client);
    ws.handshake(args.endpoint.host, args.endpoint.path);

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

}}

