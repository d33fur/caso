cmake_minimum_required(VERSION 3.16)

project(derivative LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp derivative.h)
