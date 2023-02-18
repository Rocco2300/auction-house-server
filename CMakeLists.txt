cmake_minimum_required(VERSION 3.22)
project(ah_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ah_server main.cpp)

target_link_libraries(ah_server mysqlcppconn boost_system)