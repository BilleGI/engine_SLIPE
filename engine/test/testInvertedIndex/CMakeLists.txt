cmake_minimum_required(VERSION 3.15)

project("Test class Inverted index" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(TestInvertedIndex test_inverted_index.cpp)
target_link_libraries(TestInvertedIndex PRIVATE GTest::gtest_main)

target_link_libraries(TestInvertedIndex PRIVATE  test_index)

include(GoogleTest)
gtest_discover_tests(TestInvertedIndex)

add_test(NAME TestInvertedIndex COMMAND TestInvertedIndex)

install(TARGETS TestInvertedIndex
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
