cmake_minimum_required(VERSION 3.8)

project(WindowsCPP C CXX)

find_package(ConstraintSDK REQUIRED Arithmetic SystemControls Controls)

add_executable(example main.cpp)
target_include_directories(example PRIVATE ${ConstraintSDK_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${ConstraintSDK_LIBRARIES})