cmake_minimum_required(VERSION 3.8)

project(ConstraintBasedLayout C CXX)

add_library(solver STATIC lib/solver.c)
target_include_directories(solver PUBLIC include)

add_library(constraint_arithmetic STATIC lib/constraint_arithmetic.cpp)
target_link_libraries(constraint_arithmetic PRIVATE solver)
target_include_directories(constraint_arithmetic PUBLIC include)

add_custom_target(
    example_wasm_copy
    COMMAND cmake -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm" "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm"
)

add_executable(example_wasm examples/wasm/main.cpp)
set_target_properties(example_wasm PROPERTIES SUFFIX .wasm)
set_target_properties(example_wasm PROPERTIES OUTPUT_NAME "example")
set_target_properties(example_wasm PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm")
target_link_libraries(example_wasm PRIVATE constraint_arithmetic)
target_link_options(example_wasm PRIVATE "-Wl,--export=init")
add_dependencies(example_wasm example_wasm_copy)