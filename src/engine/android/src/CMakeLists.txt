cmake_minimum_required(VERSION 2.6)
add_library (wail starter.cpp) 
target_link_libraries(wail)
include_directories(${ANDROID_INCLUDE_PATH})
