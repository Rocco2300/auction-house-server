cmake_minimum_required(VERSION 3.22)
project(ah_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

find_package(Boost REQUIRED system)
find_package(SQLite3 3.37.2 REQUIRED)
find_package(nlohmann_json 3.10.5 REQUIRED)

add_subdirectory(src/fields)
add_subdirectory(src/network)
add_subdirectory(src/entities)
add_executable(ah_server ${SOURCES} ${HEADERS})

target_link_libraries(ah_server PUBLIC fields network entities)
target_link_libraries(ah_server PRIVATE pthread Boost::system nlohmann_json::nlohmann_json sqlite3)