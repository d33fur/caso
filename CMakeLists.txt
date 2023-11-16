cmake_minimum_required(VERSION 3.16)

project(caso LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp crk4.h)

target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})