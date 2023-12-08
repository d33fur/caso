cmake_minimum_required(VERSION 3.16)

project(caso LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(tests)

find_package(Doxygen REQUIRED)

find_package(Catch2 3 REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)

add_executable(tests tests/unitTest.cpp)

add_executable(${PROJECT_NAME} main.cpp caso.h)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE cpr::cpr)
target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

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
