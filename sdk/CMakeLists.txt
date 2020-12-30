cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(ConstraintSDK)

ExternalProject_Add(
    wasm32-emscripten
    PREFIX wasm32-emscripten
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/wasm32-emscripten/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/wasm32-emscripten
    CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
    INSTALL_COMMAND ""
)

ExternalProject_Add(
    wasm32-unknown
    PREFIX wasm32-unknown
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/wasm32-unknown/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/wasm32-unknown
    CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown.cmake
    INSTALL_COMMAND ""
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/browser/
    DESTINATION lib
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/wasm32-emscripten/libsolver.a
    ${CMAKE_CURRENT_BINARY_DIR}/wasm32-emscripten/libconstraint_arithmetic.a
    DESTINATION lib/wasm32-emscripten
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/wasm32-unknown/libsolver.a
    ${CMAKE_CURRENT_BINARY_DIR}/wasm32-unknown/libconstraint_arithmetic.a
    DESTINATION lib/wasm32-unknown
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown.cmake DESTINATION cmake)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ConstraintSDKConfig.cmake DESTINATION .)