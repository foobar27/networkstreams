set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options log)

find_package(OpenSSL REQUIRED)

add_library(common STATIC
    networkstream/common/RootCertificates.cpp
    networkstream/common/RootCertificates.h
    networkstream/common/Utils.h
    networkstream/common/Utils.cpp)
target_compile_features(common PUBLIC cxx_std_17)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(client
    STATIC
    networkstream/client/Client.cpp
    networkstream/client/Client.h
    networkstream/client/Arguments.cpp
    networkstream/client/Arguments.h)
target_link_libraries(client PUBLIC
    common
    Boost::program_options
    Boost::system
    Boost::thread
    Boost::log
    OpenSSL::SSL
    OpenSSL::Crypto
    stdc++fs)

add_executable(networkstream-client
    networkstream/client/Main.cpp)

target_link_libraries(networkstream-client
    client)

add_library(server
    STATIC
    networkstream/server/Server.cpp
    networkstream/server/Server.h
    networkstream/server/ServerCertificate.h
    networkstream/server/ServerCertificate.cpp
    networkstream/server/Arguments.cpp
    networkstream/server/Arguments.h
    networkstream/server/Session.cpp
    networkstream/server/Session.h
    networkstream/server/Listener.cpp
    networkstream/server/Listener.h
)
target_compile_definitions(server PUBLIC BOOST_LOG_DYN_LINK)
target_link_libraries(server PUBLIC
    common
    Boost::program_options
    Boost::system Boost::thread
    Boost::log
    OpenSSL::SSL
    OpenSSL::Crypto
    stdc++fs)

add_executable(networkstream-server
    networkstream/server/Main.cpp)
target_link_libraries(networkstream-server
    server)
