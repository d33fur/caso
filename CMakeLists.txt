cmake_minimum_required(VERSION 3.16)

project(derivative LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
        ${INCLUDE_DIR}/*.hpp
        ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})