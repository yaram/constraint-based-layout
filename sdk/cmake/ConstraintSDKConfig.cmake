include(FindPackageHandleStandardArgs)

set(ConstraintSDK_INCLUDE_DIRS ${ConstraintSDK_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
    set(PLATFORM wasm32-emscripten)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Generic AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL wasm32)
    set(PLATFORM wasm32-unknown)
endif()

set(ConstraintSDK_LIBRARIES
    ${ConstraintSDK_LIBRARIES}
    ${ConstraintSDK_DIR}/lib/${PLATFORM}/libsolver.a
)

foreach(component ${ConstraintSDK_FIND_COMPONENTS})
    if(${component} STREQUAL Arithmetic)
        set(ConstraintSDK_LIBRARIES
            ${ConstraintSDK_LIBRARIES}
            ${ConstraintSDK_DIR}/lib/${PLATFORM}/libconstraint_arithmetic.a
        )
    elseif(${component} STREQUAL Controls)
        set(ConstraintSDK_LIBRARIES
            ${ConstraintSDK_LIBRARIES}
            ${ConstraintSDK_DIR}/lib/${PLATFORM}/libcontrols.a
        )
    endif()
endforeach()

find_package_handle_standard_args(ConstraintSDK ConstraintSDK_DIR ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)

mark_as_advanced(ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)