cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

project(ConstraintSDK)

set(PLATFORMS wasm32-unknown wasm32-emscripten)

set(PLATFORM_TOOLCHAIN_wasm32-unknown ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown.cmake)
set(PLATFORM_TOOLCHAIN_wasm32-emscripten ${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

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
        INSTALL_COMMAND ""
    )

    install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${platform}/libsolver.a
        ${CMAKE_CURRENT_BINARY_DIR}/${platform}/libconstraint_arithmetic.a
        ${CMAKE_CURRENT_BINARY_DIR}/${platform}/libcontrols.a
        DESTINATION lib/${platform}
    )
endforeach()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/browser/
    DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/wasm32-unknown.cmake DESTINATION cmake)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ConstraintSDKConfig.cmake DESTINATION .)