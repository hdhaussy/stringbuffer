cmake_minimum_required(VERSION 3.10)

# set the project name
project(stringbuffer)

# add the executable
add_executable(test test.c stringbuffer.c)
add_executable(benchmark benchmark.cpp stringbuffer.c)