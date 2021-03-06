set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options log)

find_package(OpenSSL REQUIRED)

add_executable(client
    Main.cpp
    Client.cpp
    Client.h
    Arguments.cpp
    Arguments.h
    ../common/RootCertificates.cpp
    ../common/RootCertificates.h
    ../common/Utils.h
    ../common/Utils.cpp)
target_compile_features(client PUBLIC cxx_std_17)
target_link_libraries(client PUBLIC
    Boost::program_options
    Boost::system
    Boost::thread
    Boost::log
    OpenSSL::SSL
    OpenSSL::Crypto
    stdc++fs)
