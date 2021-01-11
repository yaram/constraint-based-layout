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

struct Color {
    float red;
    float green;
    float blue;
    float alpha;
};

struct ContainerStyle {
    Color background_color;

    float border_size;
    Color border_color;
};

struct LabelStyle;
struct ButtonStyle;
struct TextInputStyle;

struct Container {
    Container *parent;

    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    bool fit_content;

    ContainerStyle *style;

    ContainerStyle *default_container_style;
    LabelStyle *default_label_style;
    ButtonStyle *default_button_style;
    TextInputStyle *default_text_input_style;

    container_t *control;
};

struct LabelStyle {
    Color text_color;
};

struct Label {
    Container *container;

    ArithmeticVariable x;
    ArithmeticVariable y;

    String text;

    String font_family;
    float font_size;

    LabelStyle *style;

    label_t *control;
};

struct ButtonStyle {
    Color text_color;

    Color background_color;

    float border_size;
    Color border_color;
};

struct Button {
    Container *container;

    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;

    ButtonStyle *style;

    button_t *control;

    void (*press_handler)();
};

struct TextInputStyle {
    Color text_color;

    Color background_color;

    float border_size;
    Color border_color;
};

struct TextInput {
    Container *container;

    ArithmeticVariable x;
    ArithmeticVariable y;

    ArithmeticVariable width;
    ArithmeticVariable height;

    String text;

    String font_family;
    float font_size;

    TextInputStyle *style;

    text_input_t *control;

    void (*change_handler)(String text);
};

struct LayoutContext {
    ArithmeticContext arithmetic_context;

    List<Container*> containers;
    List<Label*> labels;
    List<Button*> buttons;
    List<TextInput*> text_inputs;

    Color background_color { 1.0f, 1.0f, 1.0f, 1.0f };

    ContainerStyle default_container_style {
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f,
        { 0.0f, 0.0f, 0.0f, 0.0f }
    };

    LabelStyle default_label_style {
        { 0.0f, 0.0f, 0.0f, 1.0f }
    };

    ButtonStyle default_button_style {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 0.9f, 0.9f, 0.9f, 1.0f },
        1.0f,
        { 0.7f, 0.7f, 0.7f, 1.0f }
    };

    TextInputStyle default_text_input_style {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        1.0f,
        { 0.7f, 0.7f, 0.7f, 1.0f }
    };

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

Container *create_container(LayoutContext *context, Container *parent);

inline ArithmeticVariable left(Container *container) {
    return container->x;
}

inline ArithmeticVariable top(Container *container) {
    return container->y;
}

inline ArithmeticVariable width(Container *container) {
    return container->width;
}

inline ArithmeticVariable height(Container *container) {
    return container->height;
}

inline ArithmeticExpression right(Container *container) {
    return left(container) + width(container);
}

inline ArithmeticExpression bottom(Container *container) {
    return top(container) + height(container);
}

inline ArithmeticExpression horizontal_middle(Container *container) {
    return left(container) + width(container) / 2;
}

inline ArithmeticExpression vertical_middle(Container *container) {
    return top(container) + height(container) / 2;
}

Label *create_label(LayoutContext *context, Container *container, String text, String font_family, float font_size);

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

Button *create_button(LayoutContext *context, Container *container, String text, String font_family, float font_size);

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

TextInput *create_text_input(LayoutContext *context, Container *container, String text, String font_family, float font_size);

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

bool perform_layout(LayoutContext *context);