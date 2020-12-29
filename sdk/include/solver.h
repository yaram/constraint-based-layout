#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

#define coefficient_index(constraint_index, variable_index) ((constraint_index) * variable_count + (variable_index))

bool solve(
    size_t variable_count,
    size_t constraint_count,
    bool *is_variable_external,
    float *objective_constant,
    float *objective_coefficients,
    size_t *constraint_variable_indices,
    float *constraint_constants,
    float *constraint_coefficients
);

#ifdef __cplusplus
}
#endif