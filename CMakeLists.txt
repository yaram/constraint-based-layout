cmake_minimum_required(VERSION 3.8)

project(ConstraintBasedLayout C CXX)

add_library(solver STATIC lib/solver.c)
target_include_directories(solver PUBLIC include)

add_library(constraint_arithmetic STATIC lib/constraint_arithmetic.cpp)
target_link_libraries(constraint_arithmetic PRIVATE solver)
target_include_directories(constraint_arithmetic PUBLIC include)

if(${CMAKE_SYSTEM_NAME} STREQUAL Generic AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL wasm32)
    add_custom_target(
        example_wasm_cpp_copy
        COMMAND cmake -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.html"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.js"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/wasm_cpp/index.css"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/browser/controls.js"
        "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm_cpp"
    )

    add_executable(example_wasm_cpp examples/wasm_cpp/main.cpp)
    set_target_properties(example_wasm_cpp PROPERTIES SUFFIX .wasm)
    set_target_properties(example_wasm_cpp PROPERTIES OUTPUT_NAME "example")
    set_target_properties(example_wasm_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/examples/wasm_cpp")
    target_link_libraries(example_wasm_cpp PRIVATE constraint_arithmetic)
    target_link_options(example_wasm_cpp PRIVATE "-Wl,--export=init,--export=button_press_handler,--export=frame_resize_handler,--export=text_input_change_handler")
    add_dependencies(example_wasm_cpp example_wasm_cpp_copy)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
    add_custom_target(
        example_emscripten_cpp_copy
        COMMAND cmake -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp/index.html"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp/index.js"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp/index.css"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/browser/controls.js"
        "${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp"
    )

    add_executable(example_emscripten_cpp examples/emscripten_cpp/main.cpp)
    set_target_properties(example_emscripten_cpp PROPERTIES OUTPUT_NAME "example")
    set_target_properties(example_emscripten_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/examples/emscripten_cpp")
    set_target_properties(example_emscripten_cpp PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp/controls_library.js")
    target_link_libraries(example_emscripten_cpp PRIVATE constraint_arithmetic)
    target_link_options(example_emscripten_cpp PRIVATE --js-library "${CMAKE_CURRENT_SOURCE_DIR}/examples/emscripten_cpp/controls_library.js")
    target_link_options(example_emscripten_cpp PRIVATE "-s" EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\"])
    target_link_options(example_emscripten_cpp PRIVATE "-s " EXPORTED_FUNCTIONS=[\"_init\",\"_button_press_handler\",\"_frame_resize_handler\",\"_text_input_change_handler\"])
    add_dependencies(example_emscripten_cpp example_emscripten_cpp_copy)
endif()