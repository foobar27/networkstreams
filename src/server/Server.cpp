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
#include <vector>

#include "Arguments.h"
#include "Session.h"
#include "Listener.h"

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace websocket = boost::beast::websocket;

namespace networkstream { namespace server {

void run(const Arguments & args) {
    boost::asio::io_context ioc {args.threads};
    ssl::context ctx{ssl::context::sslv23};
    loadServerCertificates(ctx, args.ssl);

    std::make_shared<Listener>(ioc, ctx, tcp::endpoint{args.address, args.port})->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(args.threads - 1);
    for(auto i = args.threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();
}

}}

