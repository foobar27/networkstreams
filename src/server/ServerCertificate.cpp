#include "ServerCertificate.h"
#include "../common/Utils.h"

#include <boost/log/trivial.hpp>

void loadServerCertificate(boost::asio::ssl::context& ctx) {
    BOOST_LOG_TRIVIAL(info) << "Loading server certificate";
    std::string const cert = readFile("share/server/cert.pem");
    BOOST_LOG_TRIVIAL(info) << "Loading server key";
    std::string const key = readFile("share/server/key.pem");
    BOOST_LOG_TRIVIAL(info) << "Loading dh parameters";
    std::string const dh = readFile("share/server/dh.pem");

    ctx.set_password_callback(
        [](std::size_t,
            boost::asio::ssl::context_base::password_purpose)
        {
            return "test";
        });

    ctx.set_options(
        boost::asio::ssl::context::default_workarounds |
        boost::asio::ssl::context::no_sslv2 |
        boost::asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain(
        boost::asio::buffer(cert.data(), cert.size()));

    ctx.use_private_key(
        boost::asio::buffer(key.data(), key.size()),
        boost::asio::ssl::context::file_format::pem);

    ctx.use_tmp_dh(
        boost::asio::buffer(dh.data(), dh.size()));
}
