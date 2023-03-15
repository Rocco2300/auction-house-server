include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB HEADERS include/*.h)
include_directories(../include)

add_executable(field_test field_test.cpp ${HEADERS})
target_link_libraries(field_test GTest::gtest_main)

add_test(field_test field_test)