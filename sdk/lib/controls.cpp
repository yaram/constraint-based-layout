#include "controls.h"

bool first_layout = true;
LayoutContext global_layout_context;

Container *create_container(LayoutContext *context, Container *parent) {
    Container container {
        parent,
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        true
    };

    auto container_pointer = (Container*)allocate(sizeof(Container));
    *container_pointer = container;

    append(&context->containers, container_pointer);

    return container_pointer;
}

Label *create_label(LayoutContext *context, Container *container, String text, String font_family, float font_size) {
    Label label {
        container,
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    };

    auto label_pointer = (Label*)allocate(sizeof(Label));
    *label_pointer = label;

    append(&context->labels, label_pointer);

    return label_pointer;
}

Button *create_button(LayoutContext *context, Container *container, String text, String font_family, float font_size) {
    Button button {
        container,
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    };

    auto button_pointer = (Button*)allocate(sizeof(Button));
    *button_pointer = button;

    append(&context->buttons, button_pointer);

    return button_pointer;
}

TextInput *create_text_input(LayoutContext *context, Container *container, String text, String font_family, float font_size) {
    TextInput text_input {
        container,
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        create_new_variable(&context->arithmetic_context),
        text,
        font_family,
        font_size
    };

    auto text_input_pointer = (TextInput*)allocate(sizeof(TextInput));
    *text_input_pointer = text_input;

    append(&context->text_inputs, text_input_pointer);

    return text_input_pointer;
}

static int pack_color(Color color) {
    return (int)(color.red * 0xFF) | (int)(color.green * 0xFF) << 8 | (int)(color.blue * 0xFF) << 16 | (int)(color.alpha * 0xFF) << 24;
}

bool perform_layout(LayoutContext *context) {
    clear_controls(pack_color(context->background_color));

    for(auto container : context->containers) {
        if(container->parent != nullptr && container->parent->fit_content) {
            container->x >= 0.0f;
            container->y >= 0.0f;

            container->x + container->width <= container->parent->width;
            container->y + container->height <= container->parent->height;
        }

        if(container->fit_content) {
            arithmetic_minimize(container->width);
            arithmetic_minimize(container->height);
        }
    }

    for(auto label : context->labels) {
        if(label->container != nullptr && label->container->fit_content) {
            label->x >= 0.0f;
            label->y >= 0.0f;

            auto text_width = get_text_width(label->text, label->font_family, label->font_size);
            auto text_height = label->font_size;

            label->x + text_width <= label->container->width;
            label->y + text_height <= label->container->height;
        }
    }

    for(auto button : context->buttons) {
        if(button->container != nullptr && button->container->fit_content) {
            button->x >= 0.0f;
            button->y >= 0.0f;

            button->x + button->width <= button->container->width;
            button->y + button->height <= button->container->height;
        }
    }

    for(auto text_input : context->text_inputs) {
        if(text_input->container != nullptr && text_input->container->fit_content) {
            text_input->x >= 0.0f;
            text_input->y >= 0.0f;

            text_input->x + text_input->width <= text_input->container->width;
            text_input->y + text_input->height <= text_input->container->height;
        }
    }

    if(!solve_arithmetic_constraints(&context->arithmetic_context)) {
        return false;
    }

    while(true) {
        auto all_generated = true;

        for(auto container : context->containers) {
            if(container->control != nullptr) {
                continue;
            }

            container_t *parent_control;
            if(container->parent != nullptr) {
                if(container->parent->control == nullptr) {
                    all_generated = false;

                    continue;
                }

                parent_control = container->parent->control;
            } else {
                parent_control = nullptr;
            }

            ContainerStyle style;
            if(container->style == nullptr) {
                auto parent = container->parent;

                while(true) {
                    if(parent != nullptr) {
                        if(parent->default_container_style != nullptr) {
                            style = *parent->default_container_style;
                        } else if(parent->parent != nullptr) {
                            parent = parent->parent;

                            continue;
                        } else {
                            style = context->default_container_style;

                            break;
                        }
                    } else {
                        style = context->default_container_style;

                        break;
                    }
                }
            } else {
                style = *container->style;
            }

            container->control = create_container(
                parent_control,
                get_arithmetic_variable_value(container->x),
                get_arithmetic_variable_value(container->y),
                get_arithmetic_variable_value(container->width),
                get_arithmetic_variable_value(container->height),
                pack_color(style.background_color),
                style.border_size,
                pack_color(style.border_color)
            );
        }

        if(all_generated) {
            break;
        }
    }

    for(auto label : context->labels) {
        container_t *container_control;
        if(label->container != nullptr) {
            container_control = label->container->control;
        } else {
            container_control = nullptr;
        }

        LabelStyle style;
        if(label->style == nullptr) {
            auto container = label->container;

            while(true) {
                if(container != nullptr) {
                    if(container->default_label_style != nullptr) {
                        style = *container->default_label_style;
                    } else if(container->parent != nullptr) {
                        container = container->parent;

                        continue;
                    } else {
                        style = context->default_label_style;

                        break;
                    }
                } else {
                    style = context->default_label_style;

                    break;
                }
            }
        } else {
            style = *label->style;
        }

        label->control = create_label(
            container_control,
            get_arithmetic_variable_value(label->x),
            get_arithmetic_variable_value(label->y),
            label->text.data,
            label->text.length,
            label->font_family.data,
            label->font_family.length,
            label->font_size,
            pack_color(style.text_color)
        );
    }

    for(auto button : context->buttons) {
        container_t *container_control;
        if(button->container != nullptr) {
            container_control = button->container->control;
        } else {
            container_control = nullptr;
        }

        ButtonStyle style;
        if(button->style == nullptr) {
            auto container = button->container;

            while(true) {
                if(container != nullptr) {
                    if(container->default_button_style != nullptr) {
                        style = *container->default_button_style;
                    } else if(container->parent != nullptr) {
                        container = container->parent;

                        continue;
                    } else {
                        style = context->default_button_style;

                        break;
                    }
                } else {
                    style = context->default_button_style;

                    break;
                }
            }
        } else {
            style = *button->style;
        }

        button->control = create_button(
            container_control,
            get_arithmetic_variable_value(button->x),
            get_arithmetic_variable_value(button->y),
            get_arithmetic_variable_value(button->width),
            get_arithmetic_variable_value(button->height),
            button->text.data,
            button->text.length,
            button->font_family.data,
            button->font_family.length,
            button->font_size,
            pack_color(style.text_color),
            pack_color(style.background_color),
            style.border_size,
            pack_color(style.border_color)
        );
    }

    for(auto text_input : context->text_inputs) {
        container_t *container_control;
        if(text_input->container != nullptr) {
            container_control = text_input->container->control;
        } else {
            container_control = nullptr;
        }

        TextInputStyle style;
        if(text_input->style == nullptr) {
            auto container = text_input->container;

            while(true) {
                if(container != nullptr) {
                    if(container->default_text_input_style != nullptr) {
                        style = *container->default_text_input_style;
                    } else if(container->parent != nullptr) {
                        container = container->parent;

                        continue;
                    } else {
                        style = context->default_text_input_style;

                        break;
                    }
                } else {
                    style = context->default_text_input_style;

                    break;
                }
            }
        } else {
            style = *text_input->style;
        }

        text_input->control = create_text_input(
            container_control,
            get_arithmetic_variable_value(text_input->x),
            get_arithmetic_variable_value(text_input->y),
            get_arithmetic_variable_value(text_input->width),
            get_arithmetic_variable_value(text_input->height),
            text_input->text.data,
            text_input->text.length,
            text_input->font_family.data,
            text_input->font_family.length,
            text_input->font_size,
            pack_color(style.text_color),
            pack_color(style.background_color),
            style.border_size,
            pack_color(style.border_color)
        );
    }

    if(!first_layout) {
        deallocate((void*)global_layout_context.containers.elements);
        deallocate((void*)global_layout_context.labels.elements);
        deallocate((void*)global_layout_context.buttons.elements);
        deallocate((void*)global_layout_context.text_inputs.elements);
    }

    deallocate((void*)context->arithmetic_context.solution_variable_indices);
    deallocate((void*)context->arithmetic_context.solution_constants);

    global_layout_context = *context;

    first_layout = false;

    return true;
}

extern "C" void button_press_handler(button_t *button) {
    for(auto saved_button : global_layout_context.buttons) {
        if(saved_button->control == button) {
            if(saved_button->press_handler != nullptr) {
                saved_button->press_handler();
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

extern "C" void text_input_change_handler(text_input_t *text_input) {
    for(auto saved_text_input : global_layout_context.text_inputs) {
        if(saved_text_input->control == text_input) {
            if(saved_text_input->change_handler != nullptr) {
                auto length = get_text_input_text(saved_text_input->control, nullptr, 0);

                auto buffer = (char*)allocate(length);

                get_text_input_text(saved_text_input->control, buffer, length);

                String text {
                    buffer,
                    length
                };

                saved_text_input->change_handler(text);
            }

            break;
        }
    }
}