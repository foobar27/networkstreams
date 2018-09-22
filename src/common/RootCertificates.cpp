#include "RootCertificates.h"
#include <string>

#include "../common/Utils.h"

namespace networkstream {

void loadRootCertificateImpl(ssl::context &ctx, const std::string &fileName, boost::system::error_code &ec) {
    std::string const cert = readFile(fileName);
    ctx.add_certificate_authority(
        boost::asio::buffer(cert.data(), cert.size()), ec);
}

void loadRootCertificatesImpl(ssl::context &ctx, boost::system::error_code &ec) {
    loadRootCertificateImpl(ctx, "share/ca/digicert.crt", ec);
    if (ec)
        return;
    loadRootCertificateImpl(ctx, "share/ca/geotrust.crt", ec);
    if (ec)
        return;
}

void loadRootCertificates(ssl::context &ctx, boost::system::error_code &ec) {
    loadRootCertificatesImpl(ctx, ec);
}

void loadRootCertificates(ssl::context& ctx) {
    boost::system::error_code ec;
    loadRootCertificatesImpl(ctx, ec);
    if (ec) {
        throw boost::system::system_error{ec};
    }
}

}
