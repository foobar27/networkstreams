find_package(Catch2 REQUIRED)
add_executable(networkstream-test
    Main.cpp
    Integration.cpp)
target_link_libraries(networkstream-test Catch2 client server)
add_test(NAME networkstream-test
    COMMAND networkstream-test)
