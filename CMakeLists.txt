cmake_minimum_required(VERSION 3.6)

set(config config.cpp)

add_executable(test1 examples/test1.cpp ${config})