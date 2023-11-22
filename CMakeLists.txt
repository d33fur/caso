cmake_minimum_required(VERSION 3.16)

project(caso LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Doxygen REQUIRED)

add_executable(${PROJECT_NAME} main.cpp caso.h)

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
