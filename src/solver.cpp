#include "solver.h"

void substitute(size_t variable_count, float *constant, float *coefficients, size_t variable, float new_constant, const float *new_coefficients) {
    auto multiplyer = coefficients[variable];
    coefficients[variable] = 0.0f;

    *constant += new_constant * multiplyer;

    for(size_t i = 0; i < variable_count; i += 1) {
        if(i != variable) {
            coefficients[i] += new_coefficients[i] * multiplyer;
        }
    }
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
    while(true) {
        // Choose non-basic parameter to become basic
        auto all_basic = true;
        size_t parameter_index;

        for(size_t i = 0; i < variable_count; i += 1) {
            if(objective_coefficients[i] < 0.0) {
                all_basic = false;
                parameter_index = i;

                break;
            }
        }

        if(all_basic) {
            return true;
        }

        // Choose basic slack variable to become non-basic
        auto first = true;
        float minimum_ratio;
        size_t constraint_index;
        size_t dummy_index;

        for(size_t i = 0; i < constraint_count; i += 1) {
            auto variable_index = constraint_variable_indices[i];

            if(!is_variable_external[variable_index]) {
                auto coefficient = constraint_coefficients[coefficient_index(i, parameter_index)];

                if(coefficient < 0.0f) {
                    auto ratio = -constraint_constants[i] / coefficient;

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
        auto reciprocal_coefficient = 1.0f / constraint_coefficients[coefficient_index(constraint_index, parameter_index)];

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