cmake_minimum_required(VERSION 3.8)

project(ConstraintBasedLayout C CXX)

add_library(solver STATIC lib/solver.c)
target_include_directories(solver PUBLIC include)

add_library(constraint_arithmetic STATIC lib/constraint_arithmetic.cpp)
target_link_libraries(constraint_arithmetic PRIVATE solver)
target_include_directories(constraint_arithmetic PUBLIC include)

add_custom_target(
    example_wasm_cpp_copy
    COMMAND cmake -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.html"
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.js"
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.css"
    "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm_cpp/index.css"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/wasm_browser/controls.js"
    "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm_cpp"
)

add_executable(example_wasm_cpp examples/wasm_cpp/main.cpp)
set_target_properties(example_wasm_cpp PROPERTIES SUFFIX .wasm)
set_target_properties(example_wasm_cpp PROPERTIES OUTPUT_NAME "example")
set_target_properties(example_wasm_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm_cpp")
target_link_libraries(example_wasm_cpp PRIVATE constraint_arithmetic)
target_link_options(example_wasm_cpp PRIVATE "-Wl,--export=init,--export=button_press_handler")
add_dependencies(example_wasm_cpp example_wasm_cpp_copy)