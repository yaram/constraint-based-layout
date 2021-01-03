#include "controls.h"

bool first_layout = true;
LayoutContext global_layout_context;

static int pack_color(Color color) {
    return (int)(color.red * 0xFF) | (int)(color.green * 0xFF) << 8 | (int)(color.blue * 0xFF) << 16 | (int)(color.alpha * 0xFF) << 24;
}

void perform_layout(LayoutContext *context) {
    clear_controls(pack_color(context->background_color));

    solve_arithmetic_constraints(&context->arithmetic_context);

    for(auto &label : context->labels) {
        label.control = create_label(
            get_arithmetic_variable_value(label.x),
            get_arithmetic_variable_value(label.y),
            label.text.data,
            label.text.length,
            label.font_family.data,
            label.font_family.length,
            label.font_size,
            pack_color(label.style->text_color)
        );
    }

    for(auto &button : context->buttons) {
        button.control = create_button(
            get_arithmetic_variable_value(button.x),
            get_arithmetic_variable_value(button.y),
            get_arithmetic_variable_value(button.width),
            get_arithmetic_variable_value(button.height),
            button.text.data,
            button.text.length,
            button.font_family.data,
            button.font_family.length,
            button.font_size,
            pack_color(button.style->text_color),
            pack_color(button.style->background_color),
            button.style->border_size,
            pack_color(button.style->border_color)
        );
    }

    for(auto &text_input : context->text_inputs) {
        text_input.control = create_text_input(
            get_arithmetic_variable_value(text_input.x),
            get_arithmetic_variable_value(text_input.y),
            get_arithmetic_variable_value(text_input.width),
            get_arithmetic_variable_value(text_input.height),
            text_input.text.data,
            text_input.text.length,
            text_input.font_family.data,
            text_input.font_family.length,
            text_input.font_size,
            pack_color(text_input.style->text_color),
            pack_color(text_input.style->background_color),
            text_input.style->border_size,
            pack_color(text_input.style->border_color)
        );
    }

    if(!first_layout) {
        deallocate((void*)global_layout_context.labels.elements);
        deallocate((void*)global_layout_context.buttons.elements);
        deallocate((void*)global_layout_context.text_inputs.elements);
    }

    deallocate((void*)context->arithmetic_context.solution_variable_indices);
    deallocate((void*)context->arithmetic_context.solution_constants);

    global_layout_context = *context;

    first_layout = false;
}

extern "C" void button_press_handler(control_t *button) {
    for(auto saved_button : global_layout_context.buttons) {
        if(saved_button.control == button) {
            if(saved_button.press_handler != nullptr) {
                saved_button.press_handler();
            }

            break;
        }
    }
}

extern "C" void frame_resize_handler() {
    if(global_layout_context.frame_resize_handler != nullptr) {
        float width;
        float height;
        get_frame_size(&width, &height);

        global_layout_context.frame_resize_handler(width, height);
    }
}

extern "C" void text_input_change_handler(control_t *text_input) {
    for(auto saved_text_input : global_layout_context.text_inputs) {
        if(saved_text_input.control == text_input) {
            if(saved_text_input.change_handler != nullptr) {
                auto length = get_text_input_text(saved_text_input.control, nullptr, 0);

                auto buffer = (char*)allocate(length);

                get_text_input_text(saved_text_input.control, buffer, length);

                String text {
                    buffer,
                    length
                };

                saved_text_input.change_handler(text);
            }

            break;
        }
    }
}