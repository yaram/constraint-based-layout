#include "constraint_arithmetic.h"
#include "environment.h"

void add_arithmetic_constraint(ArithmeticExpression left, ArithmeticInequality inequality, ArithmeticExpression right) {
    if(left.context != right.context) {
        return;
    }

    auto context = left.context;

    switch(inequality) {
        case ArithmeticInequality::Equal: break;

        case ArithmeticInequality::LessThanOrEqual: {
            auto slack_variable = context->variable_count;

            context->variable_count += 1;

            context->is_variable_external = (bool*)reallocate((void*)context->is_variable_external, context->variable_count * sizeof(bool));

            context->is_variable_external[slack_variable] = false;

            auto old_coefficient_count = left.coefficient_count;

            left.coefficient_count = context->variable_count;

            left.coefficients = (float*)reallocate((void*)left.coefficients, context->variable_count * sizeof(float));

            for(size_t i = old_coefficient_count; i < slack_variable; i += 1) {
                left.coefficients[i] = 0.0f;
            }

            left.coefficients[slack_variable] = 1.0f;
        } break;

        case ArithmeticInequality::GreaterThanOrEqual: {
            auto slack_variable = context->variable_count;

            context->variable_count += 1;

            context->is_variable_external = (bool*)reallocate((void*)context->is_variable_external, context->variable_count * sizeof(bool));

            context->is_variable_external[slack_variable] = false;

            auto old_coefficient_count = left.coefficient_count;

            left.coefficient_count = context->variable_count;

            left.coefficients = (float*)reallocate((void*)left.coefficients, context->variable_count * sizeof(float));

            for(size_t i = old_coefficient_count; i < slack_variable; i += 1) {
                left.coefficients[i] = 0.0f;
            }

            left.coefficients[slack_variable] = -1.0f;
        } break;
    }

    auto constraint_index = context->constraint_count;

    context->constraint_count += 1;

    context->constraints = (ArithmeticContext::ArithmeticConstraint*)reallocate(
        (void*)context->constraints,
        context->constraint_count * sizeof(ArithmeticContext::ArithmeticConstraint)
    );

    context->constraints[constraint_index] = {
        left.constant,
        context->variable_count,
        left.coefficients,
        right.constant,
        right.coefficient_count,
        right.coefficients
    };
}

ArithmeticExpression operator+(ArithmeticExpression a, ArithmeticExpression b) {
    auto context = a.context;

    size_t coefficient_count;
    float *coefficients;
    if(a.coefficient_count > b.coefficient_count) {
        for(size_t i = 0; i < a.coefficient_count; i += 1) {
            a.coefficients[i] += b.coefficients[i];
        }

        deallocate(b.coefficients);

        coefficient_count = a.coefficient_count;
        coefficients = a.coefficients;
    } else {
        for(size_t i = 0; i < b.coefficient_count; i += 1) {
            b.coefficients[i] += a.coefficients[i];
        }

        deallocate(a.coefficients);

        coefficient_count = b.coefficient_count;
        coefficients = b.coefficients;
    }

    return {
        context,
        a.constant + b.constant,
        coefficient_count,
        coefficients
    };
}

ArithmeticExpression operator+(ArithmeticTerm a, ArithmeticTerm b) {
    auto context = a.context;

    auto coefficients = (float*)allocate(context->variable_count * sizeof(float));

    for(size_t i = 0; i < context->variable_count; i += 1) {
        coefficients[i] = 0.0f;
    }

    coefficients[a.variable_index] = a.coefficient;
    coefficients[b.variable_index] += b.coefficient;

    return {
        context,
        0.0f,
        context->variable_count,
        coefficients
    };
}

ArithmeticExpression operator+(ArithmeticTerm term, float constant) {
    auto coefficients = (float*)allocate(term.context->variable_count * sizeof(float));

    for(size_t i = 0; i < term.context->variable_count; i += 1) {
        coefficients[i] = 0.0f;
    }

    coefficients[term.variable_index] = term.coefficient;

    return {
        term.context,
        constant,
        term.context->variable_count,
        coefficients
    };
}

ArithmeticTerm::operator ArithmeticExpression() {
    auto coefficient_count = context->variable_count;
    auto coefficients = (float*)allocate(coefficient_count * sizeof(float));

    for(size_t i = 0; i < coefficient_count; i += 1) {
        coefficients[i] = 0.0f;
    }

    coefficients[variable_index] = coefficient;

    return {
        context,
        0.0f,
        coefficient_count,
        coefficients
    };
}

ArithmeticExpression operator+(ArithmeticExpression expression, ArithmeticTerm term) {
    auto context = expression.context;

    if(term.variable_index >= expression.coefficient_count) {
        auto old_coefficient_count = expression.coefficient_count;

        expression.coefficient_count = context->variable_count;

        expression.coefficients = (float*)reallocate((void*)expression.coefficients, context->variable_count * sizeof(float));

        for(size_t i = old_coefficient_count; i < context->variable_count; i += 1) {
            expression.coefficients[i] = 0.0f;
        }

        expression.coefficients[term.variable_index] = 0.0f;
    }

    expression.coefficients[term.variable_index] += term.coefficient;

    return expression;
}

ArithmeticVariable create_new_variable(ArithmeticContext *context) {
    auto index = context->variable_count;

    context->variable_count += 1;

    context->is_variable_external = (bool*)reallocate((void*)context->is_variable_external, context->variable_count * sizeof(bool));

    context->is_variable_external[index] = true;

    return {
        context,
        index
    };
}

ArithmeticExpression arithmetic_variable_to_expression(ArithmeticVariable variable) {
    auto coefficient_count = variable.context->variable_count;

    auto coefficients = (float*)allocate(coefficient_count * sizeof(float));

    for(size_t i = 0; i < coefficient_count; i += 1) {
        coefficients[i] = 0.0f;
    }

    coefficients[variable.index] = 1.0f;

    return {
        variable.context,
        0.0f,
        coefficient_count,
        coefficients
    };
}

ArithmeticSolution solve_arithmetic_constraints(ArithmeticContext context, ArithmeticExpression objective) {
    // Create full-sized tableau
    auto variable_count = context.variable_count;

    auto is_variable_external = context.is_variable_external;

    auto constraint_count = context.constraint_count;

    auto objective_constant = objective.constant;
    auto objective_coefficients = (float*)allocate(variable_count * sizeof(float));

    for(size_t i = 0; i < variable_count; i += 1) {
        objective_coefficients[i] = 0.0f;
    }

    for(size_t i = 0; i < objective.coefficient_count; i += 1) {
        objective_coefficients[i] = objective.coefficients[i];
    }

    auto constraint_constants = (float*)allocate(constraint_count * sizeof(float));

    auto constraint_coefficients = (float*)allocate(constraint_count * variable_count * sizeof(float));

    for(size_t i = 0; i < constraint_count * variable_count; i += 1) {
        constraint_coefficients[i] = 0.0f;
    }

    for(size_t i = 0; i < constraint_count; i += 1) {
        auto constraint = context.constraints[i];

        constraint_constants[i] = constraint.left_constant;
        constraint_constants[i] -= constraint.right_constant;

        for(size_t j = 0; j < constraint.left_coefficient_count; j += 1) {
            constraint_coefficients[coefficient_index(i, j)] = constraint.left_coefficients[j];
        }

        deallocate(constraint.left_coefficients);

        for(size_t j = 0; j < constraint.right_coefficient_count; j += 1) {
            constraint_coefficients[coefficient_index(i, j)] -= constraint.right_coefficients[j];
        }

        deallocate(constraint.right_coefficients);
    }

    deallocate(context.constraints);

    auto constraint_variable_indices = (size_t*)allocate(constraint_count * sizeof(size_t));

    solve(
        variable_count,
        constraint_count,
        is_variable_external,
        &objective_constant,
        objective_coefficients,
        constraint_variable_indices,
        constraint_constants,
        constraint_coefficients
    );

    deallocate(objective_coefficients);
    deallocate(constraint_coefficients);

    return {
        constraint_variable_indices,
        constraint_constants
    };
}

float get_arithmetic_variable_value(ArithmeticContext context, ArithmeticSolution solution, ArithmeticVariable variable) {
    for(size_t i = 0; i < context.variable_count; i += 1) {
        if(solution.constraint_variable_indices[i] == variable.index) {
            return solution.constraint_constants[i];

            break;
        }
    }

    return 0.0f;
}