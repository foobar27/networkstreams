cmake_minimum_required(VERSION 3.8)

project(NetworkStream VERSION 0.1
                      DESCRIPTION "A simple encrypted network stream"
                      LANGUAGES CXX)

enable_testing()

add_subdirectory(vendor/Catch2)
add_subdirectory(src/main)
add_subdirectory(src/test)
