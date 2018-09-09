set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options)

find_package(OpenSSL REQUIRED)

add_executable(server
    Main.cpp
    Utils.cpp
    Utils.h
    Server.cpp
    Server.h
    Arguments.cpp
    Arguments.h
    Session.cpp
    Session.h
    Listener.cpp
    Listener.h
    ServerCertificate.h)
target_compile_features(server PUBLIC cxx_std_11)
target_link_libraries(server PUBLIC Boost::program_options Boost::system Boost::thread OpenSSL::SSL OpenSSL::Crypto)
