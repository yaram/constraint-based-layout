cmake_minimum_required(VERSION 3.8)

project(EmscriptenCPP C CXX)

find_package(ConstraintSDK REQUIRED Arithmetic Controls)

add_custom_target(
    copy_files
    COMMAND cmake -E copy
    "\"${CMAKE_CURRENT_SOURCE_DIR}/index.html\""
    "\"${CMAKE_CURRENT_SOURCE_DIR}/index.js\""
    "\"${CMAKE_CURRENT_SOURCE_DIR}/index.css\""
    "\"${ConstraintSDK_DIR}/lib/system_controls.js\""
    "\"${CMAKE_CURRENT_BINARY_DIR}\""
)

add_executable(example main.cpp)
set_target_properties(example PROPERTIES LINK_DEPENDS "${ConstraintSDK_DIR}/lib/system_controls_emscripten.js")
target_include_directories(example PRIVATE ${ConstraintSDK_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${ConstraintSDK_LIBRARIES})
target_link_options(example PRIVATE --js-library "${ConstraintSDK_DIR}/lib/system_controls_emscripten.js")
target_link_options(example PRIVATE "-s" EXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\"])
target_link_options(example PRIVATE "-s " EXPORTED_FUNCTIONS=[\"_init\",\"_button_press_handler\",\"_frame_resize_handler\",\"_text_input_change_handler\"])
add_dependencies(example copy_files)