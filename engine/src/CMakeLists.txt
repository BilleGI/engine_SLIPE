cmake_minimum_required(VERSION 3.15)

#add_executable(search_engine main.cpp ${MY_INCLUDE_DIR}/converter_json.h converter_json.cpp ${MY_INCLUDE_DIR}/my_exception.h my_exception.cpp
#    ${MY_INCLUDE_DIR}/inverted_index.h inverted_index.cpp ${MY_INCLUDE_DIR}/search_server.h search_server.cpp)

add_executable(search_engine main.cpp)

target_link_libraries(search_engine PRIVATE engine_library)
