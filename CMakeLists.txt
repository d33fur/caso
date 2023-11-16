cmake_minimum_required(VERSION 3.16)

project(crk4 LANGUAGES CXX)
# Cauchy Runge-Kutta
set(CMAKE_CXX_STANDARD 20)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

#include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp crk4.h)

target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})