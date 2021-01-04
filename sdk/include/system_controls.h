#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct _container_t container_t;

container_t *create_container(
    container_t *parent,
    float x,
    float y,
    float width,
    float height,
    int background_color,
    float border_size,
    int border_color
);

typedef struct _label_t label_t;

label_t *create_label(
    container_t *container,
    float x,
    float y,
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size,
    int text_color
);

typedef struct _button_t button_t;

button_t *create_button(
    container_t *container,
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

void button_press_handler(button_t *button);

typedef struct _text_input_t text_input_t;

text_input_t *create_text_input(
    container_t *container,
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

size_t get_text_input_text(text_input_t *text_input, char *buffer, size_t buffer_size);

void text_input_change_handler(text_input_t *text_input);

#ifdef __cplusplus
}
#endif