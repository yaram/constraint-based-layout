include(FindPackageHandleStandardArgs)

set(ConstraintSDK_INCLUDE_DIRS ${ConstraintSDK_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
    set(PLATFORM wasm32-unknown-emscripten)
    set(PLATFORM_ARCHIVE_PREFIX lib)
    set(PLATFORM_ARCHIVE_EXTENSION a)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Generic AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL wasm32)
    set(PLATFORM wasm32-unknown-unknown)
    set(PLATFORM_ARCHIVE_PREFIX lib)
    set(PLATFORM_ARCHIVE_EXTENSION a)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Windows AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
    set(PLATFORM x86_64-pc-windows)
    set(PLATFORM_ARCHIVE_EXTENSION lib)
endif()

set(ConstraintSDK_LIBRARIES
    ${ConstraintSDK_LIBRARIES}
    ${ConstraintSDK_DIR}/lib/${PLATFORM}/${PLATFORM_ARCHIVE_PREFIX}solver.${PLATFORM_ARCHIVE_EXTENSION}
)

foreach(component ${ConstraintSDK_FIND_COMPONENTS})
    if(${component} STREQUAL Arithmetic)
        set(ConstraintSDK_LIBRARIES
            ${ConstraintSDK_LIBRARIES}
            ${ConstraintSDK_DIR}/lib/${PLATFORM}/${PLATFORM_ARCHIVE_PREFIX}constraint_arithmetic.${PLATFORM_ARCHIVE_EXTENSION}
        )
    elseif(${component} STREQUAL Controls)
        set(ConstraintSDK_LIBRARIES
            ${ConstraintSDK_LIBRARIES}
            ${ConstraintSDK_DIR}/lib/${PLATFORM}/${PLATFORM_ARCHIVE_PREFIX}controls.${PLATFORM_ARCHIVE_EXTENSION}
        )
    elseif(${component} STREQUAL SystemControls)
        set(ConstraintSDK_LIBRARIES
            ${ConstraintSDK_LIBRARIES}
            ${ConstraintSDK_DIR}/lib/${PLATFORM}/${PLATFORM_ARCHIVE_PREFIX}system_controls.${PLATFORM_ARCHIVE_EXTENSION}
        )
    endif()
endforeach()

find_package_handle_standard_args(ConstraintSDK ConstraintSDK_DIR ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)

mark_as_advanced(ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)