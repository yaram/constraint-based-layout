cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(ConstraintSDK)

set(PLATFORMS wasm32-unknown-unknown wasm32-unknown-emscripten)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows AND ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL AMD64)
    set(PLATFORMS ${PLATFORMS} x86_64-pc-windows)
endif()

set(PLATFORM_TOOLCHAIN_wasm32-unknown-unknown ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown-unknown.cmake)
set(PLATFORM_TOOLCHAIN_wasm32-unknown-emscripten ${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

foreach(platform ${PLATFORMS})
    ExternalProject_Add(
        ${platform}
        PREFIX ${platform}
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
        STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/${platform}/stamp
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${platform}
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE=${PLATFORM_TOOLCHAIN_${platform}}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/lib/${platform}
        BUILD_ALWAYS true
    )
endforeach()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/browser/
    DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown-unknown.cmake DESTINATION cmake)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ConstraintSDKConfig.cmake DESTINATION .)