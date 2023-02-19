file(GLOB SOURCES *.cpp)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(network ${SOURCES} ${HEADERS})