set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR wasm32)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_C_FLAGS "-nostdlib -ffreestanding --target=wasm32")
set(CMAKE_CXX_FLAGS "-nostdlib -ffreestanding --target=wasm32")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-entry,--import-memory,--allow-undefined")