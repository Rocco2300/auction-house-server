cmake_minimum_required(VERSION 3.22)
project(ah_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

add_subdirectory(src/network)
add_executable(ah_server ${SOURCES} ${HEADERS})

target_link_libraries(ah_server pthread mysqlcppconn boost_system network)