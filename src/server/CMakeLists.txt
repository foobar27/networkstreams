set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.67 REQUIRED COMPONENTS filesystem thread program_options)

add_executable(server Server.cpp ServerCertificate.h)
target_compile_features(server PUBLIC cxx_std_11)
target_link_libraries(server PUBLIC Boost::program_options Boost::system Boost::thread ssl crypto)
