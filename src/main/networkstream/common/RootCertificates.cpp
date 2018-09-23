#include "RootCertificates.h"
#include <string>

#include "../common/Utils.h"

namespace networkstream {

namespace ssl = boost::asio::ssl;

void addRootCertificateImpl(ssl::context & ctx, const std::string & cert, boost::system::error_code & ec) {
    ctx.add_certificate_authority(
        boost::asio::buffer(cert.data(), cert.size()), ec);
}

void adddRootCertificate(ssl::context & ctx, const std::string & cert, boost::system::error_code & ec) {
    addRootCertificateImpl(ctx, cert, ec);
}

void addRootCertificate(ssl::context & ctx, const std::string & cert) {
    boost::system::error_code ec;
    addRootCertificateImpl(ctx, cert, ec);
    if (ec) {
        throw boost::system::system_error{ec};
    }
}

}
