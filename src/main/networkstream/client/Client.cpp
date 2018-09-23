#include "Client.h"
#include "../common/RootCertificates.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>
#include <iostream>

#include "Arguments.h"

#include <boost/log/trivial.hpp>

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace networkstream { namespace client {

void Client::addRootCertificate(const std::string & cert) {
    ::networkstream::addRootCertificate(m_ctx, cert);
}

std::string Client::execute(const std::string & request) {
    BOOST_LOG_TRIVIAL(info) << "Connecting to endpoint " << m_args.endpoint;

    boost::asio::io_context ioc;

    tcp::resolver resolver{ioc};
    websocket::stream<ssl::stream<tcp::socket>> ws{ioc, m_ctx};

    auto const results = resolver.resolve(m_args.endpoint.host, m_args.endpoint.port);
    // TODO error handling
    boost::asio::connect(ws.next_layer().next_layer(), results.begin(), results.end());
    ws.next_layer().handshake(ssl::stream_base::client);
    ws.handshake(m_args.endpoint.host, m_args.endpoint.path);

    // Send the message
    ws.write(boost::asio::buffer(std::string(request)));

    // This buffer will hold the incoming message
    boost::beast::multi_buffer b;

    // Read a message into our buffer
    ws.read(b);

    // Close the WebSocket connection
    ws.close(websocket::close_code::normal);

    // If we get here then the connection is closed gracefully

    return boost::beast::buffers_to_string(b.data());
}

}}

