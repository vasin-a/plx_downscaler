cmake_minimum_required(VERSION 3.22.1)

project(downscaler)

include(shared.cmake)

add_subdirectory(ext)
add_subdirectory(lib)
add_subdirectory(host)
