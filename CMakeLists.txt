cmake_minimum_required(VERSION 3.22)
project(ah_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB HEADERS include/*.h)

add_subdirectory(src/network)
add_executable(ah_server src/main.cpp ${HEADERS})

target_link_libraries(ah_server pthread mysqlcppconn boost_system network)