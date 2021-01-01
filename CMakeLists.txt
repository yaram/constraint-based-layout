cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(ConstraintBasedLayout)

add_custom_target(examples)

ExternalProject_Add(
    sdk
    PREFIX sdk
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdk/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/sdk
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DEMSDK=${EMSDK}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(DOTNET_CONFIGURATION Debug)
else()
    set(DOTNET_CONFIGURATION ${CMAKE_BUILD_TYPE})
endif()

add_custom_target(
    sdk_nuget_package ALL
    dotnet pack
    -c ${DOTNET_CONFIGURATION}
    "\"${CMAKE_CURRENT_SOURCE_DIR}/csharp/ConstraintSDK.csproj\""
)

ExternalProject_Add(
    bare_wasm_cpp
    PREFIX examples/bare_wasm_cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/bare_wasm_cpp
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/bare_wasm_cpp/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/bare_wasm_cpp
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_BINARY_DIR}/sdk/install/cmake/wasm32-unknown.cmake
    -DConstraintSDK_DIR=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
    INSTALL_COMMAND ""
    DEPENDS sdk
    EXCLUDE_FROM_ALL true
)

add_dependencies(examples bare_wasm_cpp)

ExternalProject_Add(
    emscripten_cpp
    PREFIX examples/emscripten_cpp
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp/stamp
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
    -DConstraintSDK_DIR=${CMAKE_CURRENT_BINARY_DIR}/sdk/install
    INSTALL_COMMAND ""
    DEPENDS sdk
    EXCLUDE_FROM_ALL true
)

add_dependencies(examples emscripten_cpp)

if(NOT "${NATIVEAOT_ROOT}" STREQUAL "")
    add_custom_target(
        emscripten_csharp
        COMMAND cmake -E env "\"NATIVEAOT_ROOT=${NATIVEAOT_ROOT}\"" "\"SDK_NUGET_PACKAGE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/csharp/bin/${DOTNET_CONFIGURATION}\""
        dotnet publish
        --packages ${CMAKE_CURRENT_BINARY_DIR}/nuget_packages
        -r browser-wasm
        -c ${DOTNET_CONFIGURATION}
        -p:Platform=wasm
        "\"-p:EMSDK=${EMSDK}\""
        "\"-p:ConstraintSDKPath=${CMAKE_CURRENT_BINARY_DIR}/sdk/install\""
        "\"${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_csharp/Example.csproj\""
    )

    add_dependencies(emscripten_csharp sdk sdk_nuget_package)

    add_dependencies(examples emscripten_csharp)
endif()