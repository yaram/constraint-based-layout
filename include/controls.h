#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _control_t control_t;

float get_text_width(
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size
);

void get_frame_size(float *width, float *height);

void clear_controls();

control_t *create_label(
    float x,
    float y,
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size
);

control_t *create_button(
    float x,
    float y,
    float width,
    float height,
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size
);

void button_press_handler(control_t *button);

#ifdef __cplusplus
}
#endif