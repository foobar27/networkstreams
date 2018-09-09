#include "Session.h"
#include "Utils.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace server {

// Start the asynchronous operation
void Session::run() {
    // Perform the SSL handshake
    ws_.next_layer().async_handshake(
                ssl::stream_base::server,
                boost::asio::bind_executor(
                    strand_,
                    std::bind(
                        &Session::on_handshake,
                        shared_from_this(),
                        std::placeholders::_1)));
}

void Session::on_handshake(boost::system::error_code ec) {
    if(ec)
        return fail(ec, "handshake");

    // Accept the websocket handshake
    ws_.async_accept(
                boost::asio::bind_executor(
                    strand_,
                    std::bind(
                        &Session::on_accept,
                        shared_from_this(),
                        std::placeholders::_1)));
}

void Session::on_accept(boost::system::error_code ec) {
    if(ec)
        return fail(ec, "accept");

    // Read a message
    do_read();
}

void Session::do_read() {
    // Read a message into our buffer
    ws_.async_read(
                buffer_,
                boost::asio::bind_executor(
                    strand_,
                    std::bind(
                        &Session::on_read,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2)));
}

void Session::on_read(
        boost::system::error_code ec,
        std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if(ec == websocket::error::closed)
        return;

    if(ec)
        fail(ec, "read");

    // Echo the message
    ws_.text(ws_.got_text());
    ws_.async_write(
                buffer_.data(),
                boost::asio::bind_executor(
                    strand_,
                    std::bind(
                        &Session::on_write,
                        shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2)));
}

void Session::on_write(
        boost::system::error_code ec,
        std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}
};

