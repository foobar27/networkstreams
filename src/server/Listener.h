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

#include "Session.h"

namespace server {
// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{
    boost::asio::ssl::context& ctx_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;

public:
    Listener(
        boost::asio::io_context& ioc,
        boost::asio::ssl::context& ctx,
        boost::asio::ip::tcp::endpoint endpoint);

    // Start accepting incoming connections
    void run();

    void do_accept();

    void on_accept(boost::system::error_code ec);
};
}
