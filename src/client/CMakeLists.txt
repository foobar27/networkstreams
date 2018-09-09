set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options)

find_package(OpenSSL REQUIRED)

add_executable(client Main.cpp Client.cpp Client.h Arguments.cpp Arguments.h ../common/RootCertificates.cpp ../common/RootCertificates.h)
target_compile_features(client PUBLIC cxx_std_11)
target_link_libraries(client PUBLIC Boost::program_options Boost::system Boost::thread OpenSSL::SSL OpenSSL::Crypto)
