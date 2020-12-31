#include <stdint.h>
#include <stddef.h>
#include "debug.h"
#include "environment.h"
#include "controls.h"

const size_t max_allocator_space = 1024 * 16;

size_t next_allocation_start = 0;
uint8_t __attribute__((aligned(sizeof(size_t)))) allocator_space[max_allocator_space];

static void clear_allocator() {
    next_allocation_start = 0;
}

void *allocate(size_t size) {
    auto size_offset = next_allocation_start;

    auto data_offset = size_offset + sizeof(size_t);

    // Align to sizeof(size_t)
    auto new_next_allocation_start = ((data_offset + size) * sizeof(size_t) + sizeof(size_t) / 2) / sizeof(size_t);

    if(new_next_allocation_start > max_allocator_space) {
        return nullptr;
    }

    next_allocation_start = new_next_allocation_start;

    *(size_t*)&allocator_space[size_offset] = size;

    auto pointer = (void*)&allocator_space[data_offset];

    return pointer;
}

void *reallocate(void *pointer, size_t new_size) {
    auto new_pointer = allocate(new_size);

    if(new_pointer == nullptr) {
        return nullptr;
    }

    if(pointer != nullptr) {
        auto offset = (size_t)pointer - (size_t)allocator_space;

        auto old_size = *(size_t*)&allocator_space[offset - sizeof(size_t)];

        for(size_t i = 0; i < old_size; i += 1) {
            ((uint8_t*)new_pointer)[i] = ((uint8_t*)pointer)[i];
        }
    }

    return new_pointer;
}

void deallocate(void *pointer) {
    // Do nothing, is a simple arena allocator with no non-global deallocation/freeing
}

static size_t uint_to_string(unsigned int value, char *buffer, unsigned int radix = 10) {
    if(value == 0) {
        buffer[0] = '0';

        return 1;
    }

    size_t length = 0;

    const char digits[] = "0123456789ABCDEF";

    while(value != 0) {
        buffer[length] = digits[value % radix];

        value /= radix;

        length += 1;
    }

    for(size_t i = 0; i < length / 2; i += 1) {
        auto temp = buffer[i];
        buffer[i] = buffer[length - 1 - i];
        buffer[length - 1 - i] = temp;
    }

    return length;
}

unsigned int count = 0;

const size_t test_text_max_length = 32;
char test_text_buffer[test_text_max_length];
size_t test_text_length = 0;

float frame_width;
float frame_height;

void render();

void increment_press_handler() {
    count += 1;

    clear_allocator();

    render();
}

void frame_resize_handler(float width, float height) {
    frame_width = width;
    frame_height = height;

    clear_allocator();

    render();
}

void test_change_handler(String text) {
    for(size_t i = 0; i < text.length; i += 1) {
        if(i >= test_text_max_length) {
            break;
        }

        test_text_buffer[i] = text.data[i];
    }

    if(text.length <= test_text_max_length) {
        test_text_length = text.length;
    } else {
        test_text_length = test_text_max_length;
    }

    clear_allocator();

    render();
}

void render() {
    LayoutContext context {};

    context.frame_resize_handler = frame_resize_handler;

    char count_string[32];
    auto count_string_length = uint_to_string(count, count_string);

    auto test_label = create_label(&context, { test_text_buffer, test_text_length }, "sans-serif"_S, 20);
    auto test_label_width = get_text_width(test_label->text, test_label->font_family, test_label->font_size);
    auto test_label_height = test_label->font_size;

    auto count_label = create_label(&context, { count_string, count_string_length }, "sans-serif"_S, 20);
    auto count_label_width = get_text_width(count_label->text, count_label->font_family, count_label->font_size);
    auto count_label_height = count_label->font_size;

    auto increment_button = create_button(&context, "Increment"_S, "sans-serif"_S, 20);
    auto increment_button_text_width = get_text_width(increment_button->text, increment_button->font_family, increment_button->font_size);
    auto increment_button_text_height = increment_button->font_size;

    increment_button->press_handler = increment_press_handler;

    auto test_text_input = create_text_input(&context, { test_text_buffer, test_text_length }, "sans-serif"_S, 20);
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

extern "C" void init() {
    get_frame_size(&frame_width, &frame_height);

    render();
}

extern "C" void *memcpy(void *destination, void *source, size_t num) {
    for(size_t i = 0; i < num; i += 1) {
        ((uint8_t*)destination)[i] = ((uint8_t*)source)[i];
    }

    return destination;
}

extern "C" void *memset(void *pointer, int value, size_t count) {
    for(size_t i = 0; i < count; i += 1) {
        ((uint8_t*)pointer)[i] = (uint8_t)value;
    }

    return pointer;
}