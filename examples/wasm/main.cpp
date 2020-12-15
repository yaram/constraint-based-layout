#include <stdint.h>
#include <stddef.h>
#include "environment.h"
#include "constraint_arithmetic.h"
#include "controls.h"

const size_t max_allocator_space = 4096;

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

inline float get_label_width(Label label) {
    return get_label_width(label.text.data, label.text.length, label.font_family.data, label.font_family.length, label.font_size);
}

static control_t *solidify_label(Label label, ArithmeticContext context, ArithmeticSolution solution) {
    return create_label(
        get_arithmetic_variable_value(context, solution, label.x),
        get_arithmetic_variable_value(context, solution, label.y),
        label.text.data,
        label.text.length,
        label.font_family.data,
        label.font_family.length,
        label.font_size
    );
}

extern "C" void init() {
    ArithmeticContext context {};

    const auto width = 640.0f;
    const auto height = 480.0f;

    Label label1 {
        create_new_variable(&context),
        create_new_variable(&context),
        "Test"_S,
        "sans-serif"_S,
        20
    };

    auto label1_width = get_label_width(label1);

    Label label2 {
        create_new_variable(&context),
        create_new_variable(&context),
        "Test2"_S,
        "sans-serif"_S,
        30
    };

    auto label2_width = get_label_width(label2);

    Label label3 {
        create_new_variable(&context),
        create_new_variable(&context),
        "Test3"_S,
        "sans-serif"_S,
        40
    };

    auto label3_width = get_label_width(label3);

    label1.x <= width - label1_width;
    label1.y + label1.font_size / 2 == height / 2;

    label2.y == height - label2.font_size;

    label3.x == label1.x + -label3_width;
    label3.y + label3.font_size / 2 == label1.y + label1.font_size / 2;

    auto solution = solve_arithmetic_constraints(context, -(label1.x));

    clear_controls();

    solidify_label(label1, context, solution);
    solidify_label(label2, context, solution);
    solidify_label(label3, context, solution);

    clear_allocator();
}

extern "C" void *memset(void *pointer, int value, size_t count) {
    for(size_t i = 0; i < count; i += 1) {
        ((uint8_t*)pointer)[i] = (uint8_t)value;
    }

    return pointer;
}