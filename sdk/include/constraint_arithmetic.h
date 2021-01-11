#pragma once

#include <stddef.h>
#include "solver.h"

struct ArithmeticContext;

struct ArithmeticExpression {
    ArithmeticContext *context;

    float constant;

    size_t coefficient_count;
    float *coefficients;
};

struct ArithmeticContext {
    size_t variable_count;
    bool *is_variable_external;

    struct ArithmeticConstraint {
        float left_constant;

        size_t left_coefficient_count;
        float *left_coefficients;

        float right_constant;

        size_t right_coefficient_count;
        float *right_coefficients;
    };

    size_t constraint_count;
    ArithmeticConstraint *constraints;

    ArithmeticExpression objective;

    size_t *solution_variable_indices;
    float *solution_constants;
};

enum struct ArithmeticInequality {
    Equal,
    LessThanOrEqual,
    GreaterThanOrEqual
};

void add_arithmetic_constraint(
    ArithmeticExpression left,
    ArithmeticInequality inequality,
    ArithmeticExpression right
);

inline void operator==(ArithmeticExpression left, ArithmeticExpression right) {
    add_arithmetic_constraint(left, ArithmeticInequality::Equal, right);
}

inline void operator<=(ArithmeticExpression left, ArithmeticExpression right) {
    add_arithmetic_constraint(left, ArithmeticInequality::LessThanOrEqual, right);
}

inline void operator>=(ArithmeticExpression left, ArithmeticExpression right) {
    add_arithmetic_constraint(left, ArithmeticInequality::GreaterThanOrEqual, right);
}

inline ArithmeticExpression arithmetic_constant_to_expression(ArithmeticContext *context, float constant) {
    return {
        context,
        constant,
        0,
        nullptr
    };
}

inline void operator==(ArithmeticExpression left, float right) {
    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left == right_expression;
}

inline void operator==(float left, ArithmeticExpression right) {
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    left_expression == right;
}

inline void operator<=(ArithmeticExpression left, float right) {
    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left <= right_expression;
}

inline void operator<=(float left, ArithmeticExpression right) {
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    left_expression <= right;
}

inline void operator>=(ArithmeticExpression left, float right) {
    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left >= right_expression;
}

inline void operator>=(float left, ArithmeticExpression right) {
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    left_expression >= right;
}

ArithmeticExpression operator+(ArithmeticExpression a, ArithmeticExpression b);

inline ArithmeticExpression operator-(ArithmeticExpression expression) {
    expression.constant = -expression.constant;

    for(size_t i = 0; i < expression.coefficient_count; i += 1) {
        expression.coefficients[i] = -expression.coefficients[i];
    }

    return expression;
}

inline ArithmeticExpression operator-(ArithmeticExpression a, ArithmeticExpression b) {
    return a + -b;
}

inline ArithmeticExpression operator+(ArithmeticExpression expression, float constant) {
    expression.constant += constant;

    return expression;
}

inline ArithmeticExpression operator+(float constant, ArithmeticExpression expression) {
    return expression + constant;
}

inline ArithmeticExpression operator*(ArithmeticExpression expression, float constant) {
    expression.constant *= constant;

    for(size_t i = 0; i < expression.coefficient_count; i += 1) {
        expression.coefficients[i] *= constant;
    }

    return expression;
}

inline ArithmeticExpression operator*(float constant, ArithmeticExpression expression) {
    return expression + constant;
}

inline ArithmeticExpression operator/(ArithmeticExpression expression, float constant) {
    return expression * (1.0f / constant);
}

struct ArithmeticTerm {
    ArithmeticContext *context;

    size_t variable_index;

    float coefficient;

    operator ArithmeticExpression();
};

ArithmeticExpression operator+(ArithmeticTerm a, ArithmeticTerm b);

ArithmeticExpression operator+(ArithmeticTerm term, float constant);

inline ArithmeticExpression operator+(float constant, ArithmeticTerm term) {
    return term + constant;
}

inline ArithmeticExpression operator-(ArithmeticTerm term, float constant) {
    return term + -constant;
}

inline ArithmeticExpression operator-(float constant, ArithmeticTerm term) {
    return constant + -term;
}

inline ArithmeticTerm operator*(ArithmeticTerm term, float constant) {
    term.coefficient *= constant;

    return term;
}

inline ArithmeticTerm operator*(float constant, ArithmeticTerm term) {
    return term * constant;
}

inline ArithmeticTerm operator/(ArithmeticTerm term, float constant) {
    return term * (1.0f / constant);
}

ArithmeticExpression operator+(ArithmeticExpression expression, ArithmeticTerm term);

inline ArithmeticExpression operator+(ArithmeticTerm term, ArithmeticExpression expression) {
    return expression + term;
}

inline ArithmeticTerm operator-(ArithmeticTerm term) {
    term.coefficient = -term.coefficient;

    return term;
}

inline ArithmeticExpression operator-(ArithmeticTerm a, ArithmeticTerm b) {
    return a + -b;
}

inline ArithmeticExpression operator-(ArithmeticExpression expression, ArithmeticTerm term) {
    return expression + -term;
}

inline ArithmeticExpression operator-(ArithmeticTerm term, ArithmeticExpression expression) {
    return term + -expression;
}

struct ArithmeticVariable {
    ArithmeticContext *context;

    size_t index;

    operator ArithmeticTerm() {
        return {
            context,
            index,
            1.0f
        };
    }
};

ArithmeticVariable create_new_variable(ArithmeticContext *context);

inline ArithmeticTerm operator*(ArithmeticVariable variable, float coefficient) {
    return {
        variable.context,
        variable.index,
        coefficient
    };
}

inline ArithmeticTerm operator/(ArithmeticVariable variable, float coefficient) {
    return variable * (1.0f / coefficient);
}

ArithmeticExpression arithmetic_variable_to_expression(ArithmeticVariable variable);

inline void operator==(ArithmeticVariable left, float right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left_expression == right_expression;
}

inline void operator==(float left, ArithmeticVariable right) {    
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    auto right_expression = arithmetic_variable_to_expression(right);

    left_expression == right_expression;
}

inline void operator<=(ArithmeticVariable left, float right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left_expression <= right_expression;
}

inline void operator<=(float left, ArithmeticVariable right) {    
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    auto right_expression = arithmetic_variable_to_expression(right);

    left_expression <= right_expression;
}

inline void operator>=(ArithmeticVariable left, float right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    auto right_expression = arithmetic_constant_to_expression(left.context, right);

    left_expression >= right_expression;
}

inline void operator>=(float left, ArithmeticVariable right) {    
    auto left_expression = arithmetic_constant_to_expression(right.context, left);

    auto right_expression = arithmetic_variable_to_expression(right);

    left_expression >= right_expression;
}

inline void operator==(ArithmeticExpression left, ArithmeticVariable right) {
    auto right_expression = arithmetic_variable_to_expression(right);

    left == right_expression;
}

inline void operator==(ArithmeticVariable left, ArithmeticExpression right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    left_expression == right;
}

inline void operator<=(ArithmeticExpression left, ArithmeticVariable right) {
    auto right_expression = arithmetic_variable_to_expression(right);

    left <= right_expression;
}

inline void operator<=(ArithmeticVariable left, ArithmeticExpression right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    left_expression <= right;
}

inline void operator>=(ArithmeticExpression left, ArithmeticVariable right) {
    auto right_expression = arithmetic_variable_to_expression(right);

    left >= right_expression;
}

inline void operator>=(ArithmeticVariable left, ArithmeticExpression right) {
    auto left_expression = arithmetic_variable_to_expression(left);

    left_expression >= right;
}

bool solve_arithmetic_constraints(ArithmeticContext *context);

float get_arithmetic_variable_value(ArithmeticVariable variable);

void arithmetic_minimize(ArithmeticExpression expression);

void arithmetic_maximize(ArithmeticExpression expression);

inline void arithmetic_minimize(ArithmeticTerm term) {
    arithmetic_minimize((ArithmeticExpression)term);
}

inline void arithmetic_maximize(ArithmeticTerm term) {
    arithmetic_maximize((ArithmeticExpression)term);
}