#include "Server.h"
#include "ServerCertificate.h"

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
#include <future>
#include <chrono>

#include "Arguments.h"
#include "Session.h"
#include "Listener.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace networkstream { namespace server {

Server::Server(const Arguments & args)
    : m_ioc {args.threads}
    , m_args(args)
{
    m_threads.reserve(m_args.threads);
}

Server::~Server() {
    m_ioc.stop();
}

void Server::start() {
    loadServerCertificates(m_ctx, m_args.ssl);
    std::make_shared<Listener>(m_ioc, m_ctx, tcp::endpoint{m_args.address, m_args.port})->run();

    for(auto i = m_args.threads; i > 0; --i) {
        m_threads.emplace_back([this]
        {
            this->m_ioc.run();
        });
        m_threads.back().detach();
    }
}

void Server::awaitTerminated() {
    while (!m_ioc.stopped()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Server::stop() {
    m_ioc.stop();
    awaitTerminated();
}

void run(const Arguments & args) {
    Server server {args};
    server.start();
    server.stop();
}

}}

