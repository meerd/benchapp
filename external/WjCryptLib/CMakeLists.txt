cmake_minimum_required(VERSION 2.8)

project( WjCryptLib )

add_definitions(-include ${CMAKE_CURRENT_SOURCE_DIR}/../../ba_std.h)

# WjCryptLib Static Library
add_library( WjCryptLib STATIC
    lib/WjCryptLib_Aes.h
    lib/WjCryptLib_Aes.c
    lib/WjCryptLib_AesCbc.h
    lib/WjCryptLib_AesCbc.c
    lib/WjCryptLib_AesCtr.h
    lib/WjCryptLib_AesCtr.c
    lib/WjCryptLib_AesOfb.h
    lib/WjCryptLib_AesOfb.c
    lib/WjCryptLib_Md5.h
    lib/WjCryptLib_Md5.c
    lib/WjCryptLib_Rc4.h
    lib/WjCryptLib_Rc4.c
    lib/WjCryptLib_Sha1.h
    lib/WjCryptLib_Sha1.c
    lib/WjCryptLib_Sha256.h
    lib/WjCryptLib_Sha256.c
    lib/WjCryptLib_Sha512.h
    lib/WjCryptLib_Sha512.c )

target_include_directories( WjCryptLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib )
target_include_directories( WjCryptLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ )

set_target_properties ( WjCryptLib PROPERTIES FOLDER lib )

