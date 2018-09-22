#include "Session.h"
#include "../common/Utils.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace networkstream { namespace server {

// Start the asynchronous operation
void Session::run() {
    // Perform the SSL handshake
    m_ws.next_layer().async_handshake(
                ssl::stream_base::server,
                boost::asio::bind_executor(
                    m_strand,
                    std::bind(
                        &Session::on_handshake,
                        shared_from_this(),
                        std::placeholders::_1)));
}

void Session::on_handshake(boost::system::error_code ec) {
    if(ec)
        return fail(ec, "handshake");

    // Accept the websocket handshake
    m_ws.async_accept(
                boost::asio::bind_executor(
                    m_strand,
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
    m_ws.async_read(
                m_buffer,
                boost::asio::bind_executor(
                    m_strand,
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
    m_ws.text(m_ws.got_text());
    m_ws.async_write(
                m_buffer.data(),
                boost::asio::bind_executor(
                    m_strand,
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
    m_buffer.consume(m_buffer.size());

    // Do another read
    do_read();
}

}}

