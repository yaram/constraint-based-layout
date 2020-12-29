cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(ConstraintBasedLayout)

ExternalProject_Add(
    sdk
    PREFIX sdk
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdk/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdk
    CMAKE_ARGS
    -DEMSDK=${EMSDK}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
)

ExternalProject_Add(
    bare_wasm_cpp
    PREFIX examples/bare_wasm_cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/bare_wasm_cpp
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/bare_wasm_cpp/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/bare_wasm_cpp
    CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_BINARY_DIR}/sdk/install/cmake/wasm32-unknown.cmake
    -DConstraintSDK_DIR=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
    INSTALL_COMMAND ""
    DEPENDS sdk
)

ExternalProject_Add(
    emscripten_cpp
    PREFIX examples/emscripten_cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp
    CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
    -DConstraintSDK_DIR=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
    INSTALL_COMMAND ""
    DEPENDS sdk
)