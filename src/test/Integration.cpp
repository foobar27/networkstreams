#include <catch2/catch.hpp>

#include <networkstream/server/Server.h>
#include <networkstream/client/Client.h>
#include <networkstream/common/Utils.h>

#include <boost/log/trivial.hpp>

using namespace networkstream;

TEST_CASE( "client-server-roundtrip", "[integration]" ) {
    auto const text = "hello world";

    server::Server server {{
            boost::asio::ip::make_address("0.0.0.0"),
                    50000, // TODO use random port
                    1,
            { networkstream::readFile("../../etc/server/keys/cert.pem"),
              networkstream::readFile("../../etc/server/keys/key.pem"),
              networkstream::readFile("../../etc/server/keys/dh.pem")}}};
    BOOST_LOG_TRIVIAL(info) << "Starting server...";
    server.start();
    BOOST_LOG_TRIVIAL(info) << "Server started.";

    BOOST_LOG_TRIVIAL(info) << "Sending request";

    client::Client client {{{"localhost:50000"}}};
    client.addRootCertificate(networkstream::readFile("../../share/ca/digicert.crt"));
    client.addRootCertificate(networkstream::readFile("../../share/ca/geotrust.crt"));
    auto response = client.execute(text);
    BOOST_LOG_TRIVIAL(info) << "Got response: " << response;
    server.stop();
    REQUIRE( response == text);
}
