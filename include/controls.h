#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _control_t control_t;

void clear_controls();

control_t* create_label(
    float x,
    float y,
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size
);

void get_label_size(
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size,
    float *width,
    float *height
);

#ifdef __cplusplus
}
#endif