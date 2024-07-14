cmake_minimum_required(VERSION 3.15)

project("Search engine" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/nlohmann_json/include
)

file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(engine_library
    ${PROJECT_SOURCE_DIR}/include/inverted_index.h
    ${PROJECT_SOURCE_DIR}/include/converter_json.h
    ${PROJECT_SOURCE_DIR}/include/my_exception.h
    ${PROJECT_SOURCE_DIR}/include/search_server.h
    ${PROJECT_SOURCE_DIR}/src/inverted_index.cpp
    ${PROJECT_SOURCE_DIR}/src/converter_json.cpp
    ${PROJECT_SOURCE_DIR}/src/my_exception.cpp
    ${PROJECT_SOURCE_DIR}/src/search_server.cpp
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(TESTS "build test (or no)" ON)

    if(TESTS)
        add_library(test_index
            ${PROJECT_SOURCE_DIR}/include/inverted_index.h
            ${PROJECT_SOURCE_DIR}/src/inverted_index.cpp
        )
        enable_testing()
        add_subdirectory(test)
    endif()
endif()

#add_executable(search_engine ${all_SRCS})
add_subdirectory(src)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS search_engine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#add_subdirectory(src)

#add_subdirectory(test)

#target_include_directories(search_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
