#include "solver.h"

static void substitute(size_t variable_count, float *constant, float *coefficients, size_t variable, float new_constant, const float *new_coefficients) {
    float multiplyer = coefficients[variable];
    coefficients[variable] = 0.0f;

    *constant += new_constant * multiplyer;

    for(size_t i = 0; i < variable_count; i += 1) {
        if(i != variable) {
            coefficients[i] += new_coefficients[i] * multiplyer;
        }
    }
}

const float delta = 0.0001f;

static bool is_zero(float value) {
    return value > -delta && value < delta;
}

static bool is_positive(float value) {
    return value > delta;
}

static bool is_negative(float value) {
    return value < -delta;
}

bool solve(
    size_t variable_count,
    size_t constraint_count,
    bool *is_variable_external,
    float *objective_constant,
    float *objective_coefficients,
    size_t *constraint_variable_indices,
    float *constraint_constants,
    float *constraint_coefficients
) {
    // Transform constraints and objective into augmented simplex form
    for(size_t i = 0; i < constraint_count; i += 1) {
        bool no_variables = true;
        size_t variable_index;

        for(size_t j = 0; j < variable_count; j += 1) {
            if(is_variable_external[j] && !is_zero(constraint_coefficients[coefficient_index(i, j)])) {
                no_variables = false;
                variable_index = j;

                break;
            }
        }

        if(no_variables) {
            if(is_negative(constraint_constants[i])) {
                for(size_t j = 0; j < variable_count; j += 1) {
                    if(is_positive(constraint_coefficients[coefficient_index(i, j)])) {
                        no_variables = false;
                        variable_index = j;

                        break;
                    }
                }
            } else {
                for(size_t j = 0; j < variable_count; j += 1) {
                    if(is_negative(constraint_coefficients[coefficient_index(i, j)])) {
                        no_variables = false;
                        variable_index = j;

                        break;
                    }
                }
            }

            if(no_variables) {
                if(is_zero(constraint_constants[i])) {
                    constraint_variable_indices[i] = -1;

                    continue;
                } else {
                    return false;
                }
            }
        }

        constraint_variable_indices[i] = variable_index;

        float coefficient = -constraint_coefficients[coefficient_index(i, variable_index)];
        constraint_coefficients[coefficient_index(i, variable_index)] = 0.0f;

        float reciprocal_coefficient = 1.0f / coefficient;

        constraint_constants[i] *= reciprocal_coefficient;

        for(size_t j = 0; j < variable_count; j += 1) {
            constraint_coefficients[coefficient_index(i, j)] *= reciprocal_coefficient;
        }

        substitute(
            variable_count,
            objective_constant,
            objective_coefficients,
            variable_index,
            constraint_constants[i],
            &constraint_coefficients[i * variable_count]
        );

        for(size_t j = 0; j < constraint_count; j += 1) {
            if(j != i) {
                substitute(
                    variable_count,
                    &constraint_constants[j],
                    &constraint_coefficients[j * variable_count],
                    variable_index,
                    constraint_constants[i],
                    &constraint_coefficients[i * variable_count]
                );
            }
        }
    }

    // Minimise objective
    while(true) {
        // Choose non-basic parameter to become basic
        bool all_basic = true;
        size_t parameter_index;

        for(size_t i = 0; i < variable_count; i += 1) {
            if(is_negative(objective_coefficients[i])) {
                all_basic = false;
                parameter_index = i;

                break;
            }
        }

        if(all_basic) {
            return true;
        }

        // Choose basic slack variable to become non-basic
        bool first = true;
        float minimum_ratio;
        size_t constraint_index;
        size_t dummy_index;

        for(size_t i = 0; i < constraint_count; i += 1) {
            size_t variable_index = constraint_variable_indices[i];

            if(variable_index != -1 && !is_variable_external[variable_index]) {
                float coefficient = constraint_coefficients[coefficient_index(i, parameter_index)];

                if(is_negative(coefficient)) {
                    float ratio = -constraint_constants[i] / coefficient;

                    if(first || ratio < minimum_ratio) {
                        minimum_ratio = ratio;
                        constraint_index = i;
                        dummy_index = variable_index;

                        first = false;
                    }
                }
            }
        }

        if(first) {
            return false;
        }

        // Pivot parameter into basis and dummy out of basis

        // Change subject of constraint from dummy to parameter
        float reciprocal_coefficient = 1.0f / constraint_coefficients[coefficient_index(constraint_index, parameter_index)];

        constraint_variable_indices[constraint_index] = parameter_index;

        constraint_constants[constraint_index] = -constraint_constants[constraint_index] * reciprocal_coefficient;

        constraint_coefficients[coefficient_index(constraint_index, parameter_index)] = 0.0f;

        for(size_t i = 0; i < variable_count; i += 1) {
            if(i != parameter_index) {
                constraint_coefficients[coefficient_index(constraint_index, i)] =
                    -constraint_coefficients[coefficient_index(constraint_index, i)] * reciprocal_coefficient;
            }
        }

        constraint_coefficients[coefficient_index(constraint_index, dummy_index)] = 1.0f * reciprocal_coefficient;

        // Substitute parameter for new constraint expression in all other constraints and objective

        for(size_t i = 0; i < constraint_count; i += 1) {
            if(i != constraint_index) {
                substitute(
                    variable_count,
                    &constraint_constants[i],
                    &constraint_coefficients[i * variable_count],
                    parameter_index,
                    constraint_constants[constraint_index],
                    &constraint_coefficients[constraint_index * variable_count]
                );
            }
        }

        substitute(
            variable_count,
            objective_constant,
            objective_coefficients,
            parameter_index,
            constraint_constants[constraint_index],
            &constraint_coefficients[constraint_index * variable_count]
        );
    }
}