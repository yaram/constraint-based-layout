#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _control_t control_t;

float get_text_width(
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size
);

void get_frame_size(float *width, float *height);

void frame_resize_handler();

void clear_controls(int background_color);

control_t *create_label(
    float x,
    float y,
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size,
    int text_color
);

control_t *create_button(
    float x,
    float y,
    float width,
    float height,
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size,
    int text_color,
    int background_color,
    float border_size,
    int border_color
);

void button_press_handler(control_t *button);

control_t *create_text_input(
    float x,
    float y,
    float width,
    float height,
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size,
    int text_color,
    int background_color,
    float border_size,
    int border_color
);

size_t get_text_input_text(control_t *text_input, char *buffer, size_t buffer_size);

void text_input_change_handler(control_t *text_input);

#ifdef __cplusplus
}
#endif