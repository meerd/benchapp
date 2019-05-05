cmake_minimum_required(VERSION 2.8)
set(APP_NAME	benchapp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BENCHAPP_CONFIG}")

message(STATUS "CONFIG " ${BENCHAPP_CONFIG})

include_directories(${APP_NAME} ${CMAKE_SOURCE_DIR}/external/WjCryptLib/lib)

add_executable(${APP_NAME}                
                main.c
                ba_md5.c
                ba_port.c
)

target_link_libraries(${APP_NAME} ${CMAKE_SOURCE_DIR}/external/WjCryptLib/build/libWjCryptLib.a)

project(benchapp)

