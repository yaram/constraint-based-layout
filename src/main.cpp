#include <stdint.h>
#include <stddef.h>
#include "environment.h"
#include "constraint_arithmetic.h"

extern "C" void environment_panic();

[[noreturn]] void panic() {
    while(true) {
        environment_panic();
    }
}

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
    const char *text;
    size_t length;
};

static String operator ""_S(const char *data, size_t length) {
    return {
        data,
        length
    };
}

struct Control;

extern "C" void environment_panic();

extern "C" float get_text_width(const char* text_data, size_t text_length, const char* font_family_data, size_t font_family_length, float font_size);

inline float get_text_width(String text, String font_family, float font_size) {
    return get_text_width(text.text, text.length, font_family.text, font_family.length, font_size);
}

extern "C" Control* create_label(
    float x,
    float y,
    const char* text_data,
    size_t text_length,
    const char* font_family_data,
    size_t font_family_length,
    float font_size
);

inline Control* create_label(float x, float y, String text, String font_family, float font_size) {
    return create_label(x, y, text.text, text.length, font_family.text, font_family.length, font_size);
}

extern "C" void set_position(Control *control, float x, float y);

struct Label {
    ArithmeticVariable x;
    ArithmeticVariable y;

    String text;

    String font_family;
    float font_size;
};

static Control *solidify_label(Label label, ArithmeticContext context, ArithmeticSolution solution) {
    return create_label(
        get_arithmetic_variable_value(context, solution, label.x),
        get_arithmetic_variable_value(context, solution, label.y),
        label.text,
        label.font_family,
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

    auto label1_width = get_text_width(label1.text, label1.font_family, label1.font_size);

    Label label2 {
        create_new_variable(&context),
        create_new_variable(&context),
        "Test2"_S,
        "sans-serif"_S,
        20
    };

    auto label2_width = get_text_width(label2.text, label2.font_family, label2.font_size);

    Label label3 {
        create_new_variable(&context),
        create_new_variable(&context),
        "Test3"_S,
        "sans-serif"_S,
        20
    };

    auto label3_width = get_text_width(label3.text, label3.font_family, label3.font_size);

    label1.x <= width - label1_width;
    label1.y + label1.font_size / 2 == height / 2;

    label2.y == height - label2.font_size;

    label3.x == label1.x + -label3_width;
    label3.y == label1.y + -10;

    auto solution = solve_arithmetic_constraints(context, -(label1.x));

    solidify_label(label1, context, solution);
    solidify_label(label2, context, solution);
    solidify_label(label3, context, solution);

    clear_allocator();
}