cmake_minimum_required(VERSION 2.8)
set(APP_NAME	benchapp)

include_directories(${APP_NAME} ${CMAKE_SOURCE_DIR}/external/WjCryptLib/lib)

add_executable(${APP_NAME} main.c)

target_link_libraries(${APP_NAME} ${CMAKE_SOURCE_DIR}/external/WjCryptLib/build/libWjCryptLib.a)

project(benchapp)

