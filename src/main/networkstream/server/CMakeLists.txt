set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_LOG_DYN_LINK ON)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options log)

find_package(OpenSSL REQUIRED)

add_executable(server
    Main.cpp
    Server.cpp
    Server.h
    Arguments.cpp
    Arguments.h
    Session.cpp
    Session.h
    Listener.cpp
    Listener.h
    ServerCertificate.h
    ServerCertificate.cpp
     ../common/Utils.h
     ../common/Utils.cpp)
target_compile_features(server PUBLIC cxx_std_17)
target_compile_definitions(server PUBLIC BOOST_LOG_DYN_LINK)
target_link_libraries(server PUBLIC
    Boost::program_options
    Boost::system Boost::thread
    Boost::log
    OpenSSL::SSL
    OpenSSL::Crypto
    stdc++fs)
