#pragma once

#include <stddef.h>

#define coefficient_index(constraint_index, variable_index) ((constraint_index) * variable_count + (variable_index))

void substitute(size_t variable_count, float *constant, float *coefficients, size_t variable, float new_constant, const float *new_coefficients);

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