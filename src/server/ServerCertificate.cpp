#include "ServerCertificate.h"
#include "../common/Utils.h"

#include <boost/log/trivial.hpp>

namespace server {

void loadServerCertificates(boost::asio::ssl::context& ctx, const SSLArguments & args) {
    const auto & cert = args.sslCertificate;
    const auto & key = args.sslKey;
    const auto & dh = args.sslDh;

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

}
