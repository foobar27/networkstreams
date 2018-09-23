find_package(Catch2 REQUIRED)

add_executable(networkstream-test
    Main.cpp
    Integration.cpp)

#
#TODO this should not be neccessary?
#
find_package(OpenSSL REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.68 REQUIRED COMPONENTS filesystem thread program_options log)
#target_compile_definitions(networkstream-test PUBLIC BOOST_LOG_DYN_LINK)
target_link_libraries(networkstream-test PUBLIC
    common
    Boost::program_options
    Boost::system
    Boost::thread
    Boost::log
    OpenSSL::SSL
    OpenSSL::Crypto
    stdc++fs)

target_link_libraries(networkstream-test PUBLIC Catch2 client server)
add_test(NAME networkstream-test
    COMMAND networkstream-test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
