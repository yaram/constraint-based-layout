#pragma once

#include "constraint_arithmetic.h"
#include "system_controls.h"
#include "list.h"

struct String {
    const char *data;

    size_t length;
};

inline String operator ""_S(const char *data, size_t length) {
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

    control_t *control;
};

struct Button {
    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;

    control_t *control;

    void (*press_handler)();
};

struct TextInput {
    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;

    control_t *control;

    void (*change_handler)(String text);
};

struct LayoutContext {
    ArithmeticContext arithmetic_context;

    List<Label> labels;
    List<Button> buttons;
    List<TextInput> text_inputs;

    void (*frame_resize_handler)(float width, float height);
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

inline Label *create_label(LayoutContext *context, String text, String font_family, float font_size) {
    auto index = append(&context->labels, {
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    });

    return &context->labels[index];
}

inline ArithmeticVariable left(Label *label) {
    return label->x;
}

inline ArithmeticVariable top(Label *label) {
    return label->y;
}

inline float width(Label *label) {
    return get_text_width(label->text, label->font_family, label->font_size);
}

inline float height(Label *label) {
    return label->font_size;
}

inline ArithmeticExpression right(Label *label) {
    return left(label) + width(label);
}

inline ArithmeticExpression bottom(Label *label) {
    return top(label) + height(label);
}

inline ArithmeticExpression horizontal_middle(Label *label) {
    return left(label) + width(label) / 2;
}

inline ArithmeticExpression vertical_middle(Label *label) {
    return top(label) + height(label) / 2;
}

inline Button *create_button(LayoutContext *context, String text, String font_family, float font_size) {
    auto index = append(&context->buttons, {
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    });

    return &context->buttons[index];
}

inline ArithmeticVariable left(Button *button) {
    return button->x;
}

inline ArithmeticVariable top(Button *button) {
    return button->y;
}

inline ArithmeticVariable width(Button *button) {
    return button->width;
}

inline ArithmeticVariable height(Button *button) {
    return button->height;
}

inline ArithmeticExpression right(Button *button) {
    return left(button) + width(button);
}

inline ArithmeticExpression bottom(Button *button) {
    return top(button) + height(button);
}

inline ArithmeticExpression horizontal_middle(Button *button) {
    return left(button) + width(button) / 2;
}

inline ArithmeticExpression vertical_middle(Button *button) {
    return top(button) + height(button) / 2;
}

inline TextInput *create_text_input(LayoutContext *context, String text, String font_family, float font_size) {
    auto index = append(&context->text_inputs, {
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    });

    return &context->text_inputs[index];
}

inline ArithmeticVariable left(TextInput *text_input) {
    return text_input->x;
}

inline ArithmeticVariable top(TextInput *text_input) {
    return text_input->y;
}

inline ArithmeticVariable width(TextInput *text_input) {
    return text_input->width;
}

inline ArithmeticVariable height(TextInput *text_input) {
    return text_input->height;
}

inline ArithmeticExpression right(TextInput *text_input) {
    return left(text_input) + width(text_input);
}

inline ArithmeticExpression bottom(TextInput *text_input) {
    return top(text_input) + height(text_input);
}

inline ArithmeticExpression horizontal_middle(TextInput *text_input) {
    return left(text_input) + width(text_input) / 2;
}

inline ArithmeticExpression vertical_middle(TextInput *text_input) {
    return top(text_input) + height(text_input) / 2;
}

void perform_layout(LayoutContext *context);