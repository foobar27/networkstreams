#include "Listener.h"
#include "Utils.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace server {
// Accepts incoming connections and launches the sessions
Listener::Listener(
        boost::asio::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint)
    : ctx_(ctx)
    , acceptor_(ioc)
    , socket_(ioc) {
    boost::system::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if(ec)
    {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if(ec) {
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(
                boost::asio::socket_base::max_listen_connections, ec);
    if(ec) {
        fail(ec, "listen");
        return;
    }
}

// Start accepting incoming connections
void
Listener::run() {
    if (!acceptor_.is_open())
        return;
    do_accept();
}

void
Listener::do_accept() {
    acceptor_.async_accept(
                socket_,
                std::bind(
                    &Listener::on_accept,
                    shared_from_this(),
                    std::placeholders::_1));
}

void
Listener::on_accept(boost::system::error_code ec) {
    if(ec) {
        fail(ec, "accept");
        exit(-1);
    } else {
        // Create the session and run it
        std::make_shared<Session>(std::move(socket_), ctx_)->run();
    }

    // Accept another connection
    do_accept();
}

}
