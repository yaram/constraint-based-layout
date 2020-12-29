#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "environment.h"
#include "constraint_arithmetic.h"
#include "controls.h"

struct String {
    const char *data;
    size_t length;
};

static String operator ""_S(const char *data, size_t length) {
    return {
        data,
        length
    };
}

struct Label {
    ArithmeticVariable x;
    ArithmeticVariable y;

    String text;

    String font_family;
    float font_size;
};

static Label create_label(ArithmeticContext *context, String text, String font_family, float font_size) {
    return {
        create_new_variable(context),
        create_new_variable(context),
        text,
        font_family,
        font_size
    };
}

struct Size {
    float width;
    float height;
};

inline float get_text_width(String text, String font_family, float font_size) {
    return get_text_width(
        text.data,
        text.length,
        font_family.data,
        font_family.length,
        font_size
    );
}

inline Size get_frame_size() {
    float width;
    float height;
    get_frame_size(&width, &height);

    return {
        width,
        height
    };
}

static control_t *solidify_label(Label label) {
    return create_label(
        get_arithmetic_variable_value(label.x),
        get_arithmetic_variable_value(label.y),
        label.text.data,
        label.text.length,
        label.font_family.data,
        label.font_family.length,
        label.font_size
    );
}

struct Button {
    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;
};

static Button create_button(ArithmeticContext *context, String text, String font_family, float font_size) {
    return {
        create_new_variable(context),
        create_new_variable(context),
        create_new_variable(context),
        create_new_variable(context),
        text,
        font_family,
        font_size
    };
}

static control_t *solidify_button(Button button) {
    return create_button(
        get_arithmetic_variable_value(button.x),
        get_arithmetic_variable_value(button.y),
        get_arithmetic_variable_value(button.width),
        get_arithmetic_variable_value(button.height),
        button.text.data,
        button.text.length,
        button.font_family.data,
        button.font_family.length,
        button.font_size
    );
}

struct TextInput {
    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;
};

static TextInput create_text_input(ArithmeticContext *context, String text, String font_family, float font_size) {
    return {
        create_new_variable(context),
        create_new_variable(context),
        create_new_variable(context),
        create_new_variable(context),
        text,
        font_family,
        font_size
    };
}

static control_t *solidify_text_input(TextInput text_input) {
    return create_text_input(
        get_arithmetic_variable_value(text_input.x),
        get_arithmetic_variable_value(text_input.y),
        get_arithmetic_variable_value(text_input.width),
        get_arithmetic_variable_value(text_input.height),
        text_input.text.data,
        text_input.text.length,
        text_input.font_family.data,
        text_input.font_family.length,
        text_input.font_size
    );
}

unsigned int count = 0;

char *test_text;
size_t test_text_length = 0;

void render() {
    ArithmeticContext context {};

    auto frame_size = get_frame_size();

    char count_string[32];
    auto count_string_length = (size_t)sprintf(count_string, "%u", count);

    auto test_label = create_label(&context, { test_text, test_text_length }, "sans-serif"_S, 20);
    auto test_label_width = get_text_width(test_label.text, test_label.font_family, test_label.font_size);
    auto test_label_height = test_label.font_size;

    auto count_label = create_label(&context, { count_string, count_string_length }, "sans-serif"_S, 20);
    auto count_label_width = get_text_width(count_label.text, count_label.font_family, count_label.font_size);
    auto count_label_height = count_label.font_size;

    auto increment_button = create_button(&context, "Increment"_S, "sans-serif"_S, 20);
    auto increment_button_text_width = get_text_width(increment_button.text, increment_button.font_family, increment_button.font_size);
    auto increment_button_text_height = increment_button.font_size;

    auto test_text_input = create_text_input(&context, { test_text, test_text_length }, "sans-serif"_S, 20);
    auto test_text_input_text_height = test_text_input.font_size;

    test_text_input.width == frame_size.width / 3;
    test_text_input.height == test_text_input_text_height + 8 * 2;

    test_text_input.x + test_text_input.width / 2 == increment_button.x + increment_button.width / 2;
    test_text_input.y == increment_button.y + increment_button.height + 8;

    increment_button.width <= frame_size.width / 2;
    increment_button.height == increment_button_text_height + 8 * 2;

    count_label.x + count_label_width / 2 == increment_button.x + increment_button.width / 2;
    count_label.y + count_label_height == increment_button.y - 8;

    test_label.x + test_label_width / 2 == count_label.x + count_label_width / 2;
    test_label.y + test_label_height == count_label.y - 8;

    increment_button.x + increment_button.width / 2 == frame_size.width / 2;

    increment_button.y - (increment_button.y - (count_label.y + count_label_height)) / 2 == frame_size.height / 2;

    solve_arithmetic_constraints(&context);

    clear_controls();

    solidify_label(test_label);
    solidify_label(count_label);
    solidify_button(increment_button);
    solidify_text_input(test_text_input);

    free(context.solution_constants);
    free(context.solution_variable_indices);
}

extern "C" void init() {
    render();
}

extern "C" void button_press_handler(control_t *button) {
    count += 1;

    render();
}

extern "C" void frame_resize_handler() {
    render();
}

extern "C" void text_input_change_handler(control_t *text_input) {    
    test_text_length = get_text_input_text(text_input, nullptr, 0);

    test_text = (char*)realloc(test_text, test_text_length);

    get_text_input_text(text_input, test_text, test_text_length);

    render();
}