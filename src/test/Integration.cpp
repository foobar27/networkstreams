#include <catch2/catch.hpp>

#include <networkstream/client/Client.h>

using namespace networkstream;

TEST_CASE( "client-server-roundtrip", "[integration]" ) {
    auto const text = "hello world";
    client::Arguments args { {"localhost:50000"}, false};
    auto response = client::run(args, text);
    std::cout << "got response: " << run(args, text) << std::endl;
    REQUIRE( response == text);
}
