file(GLOB SOURCES *.cpp)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(entities ${SOURCES} ${HEADERS})

target_link_libraries(entities PUBLIC fields)