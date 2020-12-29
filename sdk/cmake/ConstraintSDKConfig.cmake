include(FindPackageHandleStandardArgs)

set(ConstraintSDK_INCLUDE_DIRS ${ConstraintSDK_DIR}/include)

if(${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
    set(ConstraintSDK_LIBRARIES
        ${ConstraintSDK_LIBRARIES}
        ${ConstraintSDK_DIR}/lib/wasm32-emscripten/libsolver.a
    )

    foreach(component ${ConstraintSDK_FIND_COMPONENTS})
        if(${component} STREQUAL Arithmetic)
            set(ConstraintSDK_LIBRARIES
                ${ConstraintSDK_LIBRARIES}
                ${ConstraintSDK_DIR}/lib/wasm32-emscripten/libconstraint_arithmetic.a
            )
        endif()
    endforeach()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Generic AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL wasm32)
    set(ConstraintSDK_LIBRARIES
        ${ConstraintSDK_LIBRARIES}
        ${ConstraintSDK_DIR}/lib/wasm32-unknown/libsolver.a
    )

    foreach(component ${ConstraintSDK_FIND_COMPONENTS})
        if(${component} STREQUAL Arithmetic)
            set(ConstraintSDK_LIBRARIES
                ${ConstraintSDK_LIBRARIES}
                ${ConstraintSDK_DIR}/lib/wasm32-unknown/libconstraint_arithmetic.a
            )
        endif()
    endforeach()
endif()

find_package_handle_standard_args(ConstraintSDK ConstraintSDK_DIR ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)

mark_as_advanced(ConstraintSDK_LIBRARIES ConstraintSDK_INCLUDE_DIRS)