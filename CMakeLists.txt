cmake_minimum_required(VERSION 3.16)

project(caso LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

find_package(Doxygen REQUIRED)

add_executable(${PROJECT_NAME} main.cpp caso.h)

target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})

if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    add_custom_target(
            documentation
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM
    )
endif()