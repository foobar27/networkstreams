#include "RootCertificates.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

std::string readFile(const std::string &name) {
    std::ifstream input(name);
    std::stringstream sstr;

    while(input >> sstr.rdbuf());
    return sstr.str();
}

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
