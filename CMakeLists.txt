cmake_minimum_required(VERSION 3.16)

project(crk4 LANGUAGES CXX)
# Cauchy Runge-Kutta
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp crk4.h)
