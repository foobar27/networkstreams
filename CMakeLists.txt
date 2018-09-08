cmake_minimum_required(VERSION 3.8)

project(NetworkStream VERSION 0.1
                      DESCRIPTION "A simple encrypted network stream"
                      LANGUAGES CXX)

		    
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_BEAST_USE_OPENSSL ON)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options)

add_executable(server server.cpp server_certificate.h)
target_compile_features(server PUBLIC cxx_std_11)
target_link_libraries(server PUBLIC Boost::program_options Boost::system Boost::thread ssl crypto)


add_executable(client client.cpp RootCertificates.cpp RootCertificates.h)
target_compile_features(client PUBLIC cxx_std_11)
target_link_libraries(client PUBLIC Boost::program_options Boost::system Boost::thread ssl crypto)
