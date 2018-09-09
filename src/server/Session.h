#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>


namespace server {

// Echoes back all received WebSocket messages
class Session : public std::enable_shared_from_this<Session> {
    boost::asio::ip::tcp::socket socket_;
    boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket&>> ws_;
    boost::asio::strand<
        boost::asio::io_context::executor_type> strand_;
    boost::beast::multi_buffer buffer_;

public:
    // Take ownership of the socket
    Session(boost::asio::ip::tcp::socket socket, boost::asio::ssl::context& ctx)
        : socket_(std::move(socket))
        , ws_(socket_, ctx)
        , strand_(ws_.get_executor())
    {}

    // Start the asynchronous operation
    void run();

    void on_handshake(boost::system::error_code ec);

    void on_accept(boost::system::error_code ec);

    void do_read();

    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);

    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);
};

}
