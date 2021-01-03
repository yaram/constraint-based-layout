#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "environment.h"
#include "controls.h"
#include "system_controls_win32.h"

unsigned int count = 0;

String test_text;

float frame_width;
float frame_height;

void render();

void increment_press_handler() {
    count += 1;

    render();
}

void frame_resize_handler(float width, float height) {
    frame_width = width;
    frame_height = height;

    render();
}

void test_change_handler(String text) {
    free((void*)test_text.data);

    test_text = text;

    render();
}

void render() {
    LayoutContext context {};

    context.frame_resize_handler = frame_resize_handler;

    char count_string[32];
    auto count_string_length = (size_t)sprintf(count_string, "%u", count);

    auto test_label = create_label(&context, test_text, "sans-serif"_S, 20);
    auto test_label_width = get_text_width(test_label->text, test_label->font_family, test_label->font_size);
    auto test_label_height = test_label->font_size;

    auto count_label = create_label(&context, { count_string, count_string_length }, "sans-serif"_S, 20);
    auto count_label_width = get_text_width(count_label->text, count_label->font_family, count_label->font_size);
    auto count_label_height = count_label->font_size;

    auto increment_button = create_button(&context, "Increment"_S, "sans-serif"_S, 20);
    auto increment_button_text_width = get_text_width(increment_button->text, increment_button->font_family, increment_button->font_size);
    auto increment_button_text_height = increment_button->font_size;

    increment_button->press_handler = increment_press_handler;

    auto test_text_input = create_text_input(&context, test_text, "sans-serif"_S, 20);
    auto test_text_input_text_height = test_text_input->font_size;

    test_text_input->change_handler = test_change_handler;

    auto padding = 8.0f;

    width(test_text_input) == frame_width / 3;
    height(test_text_input) == test_text_input->font_size + padding * 2;

    horizontal_middle(test_text_input) == horizontal_middle(increment_button);
    top(test_text_input) == bottom(increment_button) + padding;

    width(increment_button) == frame_width / 2;
    height(increment_button) == increment_button->font_size + padding * 2;

    horizontal_middle(increment_button) == frame_width / 2;
    top(increment_button) - (top(increment_button) - bottom(count_label)) / 2 == frame_height / 2;

    horizontal_middle(count_label) == horizontal_middle(increment_button);
    bottom(count_label) == top(increment_button) - padding;

    horizontal_middle(test_label) == horizontal_middle(increment_button);
    bottom(test_label) == top(count_label) - padding;

    perform_layout(&context);
}

int main(int argc, char *argv[]) {
    init_system_controls();

    get_frame_size(&frame_width, &frame_height);

    render();

    while(wait_for_events()) {}

    return 0;
}