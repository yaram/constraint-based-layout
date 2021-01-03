#include "system_controls.h"
#include <Windows.h>
#include "list.h"

struct Control {
    size_t index;

    HWND window;
};

HDC device_context;
HWND window;

size_t next_control_index = 0;
List<Control> controls {};

inline void wide_string_to_utf8(const wchar_t *text_data, char **result_data, size_t *result_length) {
    auto length = WideCharToMultiByte(CP_UTF8, 0, text_data, wcslen(text_data), nullptr, 0, nullptr, nullptr);

    auto buffer = (char*)malloc(length);

    WideCharToMultiByte(CP_UTF8, 0, text_data, wcslen(text_data), buffer, length, nullptr, nullptr);

    *result_data = buffer;
    *result_length = length;
}

inline void utf8_to_wide_string(const char *text_data, size_t text_length, wchar_t **result_data, size_t *result_length) {
    auto length = MultiByteToWideChar(CP_UTF8, 0, text_data, text_length, nullptr, 0);

    auto buffer = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));

    MultiByteToWideChar(CP_UTF8, 0, text_data, text_length, buffer, length);

    buffer[length] = 0;

    *result_data = buffer;
    *result_length = length;
}

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM wide_parameter, LPARAM long_parameter) {
    switch(message) {
        case WM_COMMAND: {
            switch(HIWORD(wide_parameter)) {
                case BN_CLICKED: {
                    for(auto control : controls) {
                        if(control.window == (HWND)long_parameter) {
                            button_press_handler((control_t*)control.index);

                            break;
                        }
                    }

                    return 0;
                } break;

                case EN_KILLFOCUS: {
                    for(auto control : controls) {
                        if(control.window == (HWND)long_parameter) {
                            text_input_change_handler((control_t*)control.index);

                            break;
                        }
                    }

                    return 0;
                } break;

                default: {
                    return DefWindowProcW(window, message, wide_parameter, long_parameter);
                }
            }
        } break;

        case WM_ERASEBKGND: {
            auto device_context = (HDC)wide_parameter;

            RECT rect;
            GetClientRect(window, &rect);

            FillRect(device_context, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

            return 0;
        } break;

        case WM_SIZE: {
            frame_resize_handler();

            return DefWindowProcW(window, message, wide_parameter, long_parameter);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);

            return 0;
        } break;

        default: {
            return DefWindowProcW(window, message, wide_parameter, long_parameter);
        }
    }
}

extern "C" void init_system_controls() {
    auto module = GetModuleHandleA(nullptr);

    auto class_name = L"ConstraintSDKClass";

    WNDCLASSW window_class_info {};
    window_class_info.style = CS_OWNDC;
    window_class_info.hInstance = module;
    window_class_info.lpszClassName = class_name;
    window_class_info.lpfnWndProc = window_procedure;
    window_class_info.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));

    RegisterClassW(&window_class_info);

    window = CreateWindowExW(
        0,
        class_name,
        L"Constraint SDK",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        nullptr,
        nullptr,
        module,
        nullptr
    );

    device_context = GetDC(window);
}

extern "C" bool wait_for_events() {
    MSG message;
    if(GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);

        return true;
    }

    return false;
}

static HFONT get_windows_font(const char *font_family_data, size_t font_family_length, int font_size) {
    if(strncmp("serif", font_family_data, font_family_length) == 0) {
        font_family_data = "Times New Roman";
        font_family_length = strlen(font_family_data);
    } else if(strncmp("sans-serif", font_family_data, font_family_length) == 0) {
        font_family_data = "Arial";
        font_family_length = strlen(font_family_data);
    }

    wchar_t *wide_font_family_data;
    size_t wide_font_family_length;
    utf8_to_wide_string(font_family_data, font_family_length, &wide_font_family_data, &wide_font_family_length);

    auto font = CreateFontW(
        font_size,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH,
        wide_font_family_data
    );

    free(wide_font_family_data);

    return font;
}

extern "C" float get_text_width(
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size
) {
    if(text_length == 0) {
        return 0.0f;
    }

    auto font = get_windows_font(font_family_data, font_family_length, (int)font_size);

    auto original_font = SelectObject(device_context, font);

    wchar_t *wide_data;
    size_t wide_length;
    utf8_to_wide_string(text_data, text_length, &wide_data, &wide_length);

    SIZE text_size;
    GetTextExtentPoint32W(device_context, wide_data, wide_length, &text_size);

    free(wide_data);

    SelectObject(device_context, original_font);

    DeleteObject(font);

    return (float)text_size.cx;
}

extern "C" void get_frame_size(float *width, float *height) {
    RECT client_rect;
    GetClientRect(window, &client_rect);

    *width = (float)(client_rect.right - client_rect.left);
    *height = (float)(client_rect.bottom - client_rect.top);
}

extern "C" void clear_controls(int background_color) {
    for(auto control : controls) {
        DestroyWindow(control.window);
    }

    controls.count = 0;
}

extern "C" control_t *create_label(
    float x,
    float y,
    const char *text_data,
    size_t text_length,
    const char *font_family_data,
    size_t font_family_length,
    float font_size,
    int text_color
) {
    auto font = get_windows_font(font_family_data, font_family_length, (int)font_size);

    wchar_t *wide_data;
    size_t wide_length;
    utf8_to_wide_string(text_data, text_length, &wide_data, &wide_length);

    auto original_font = SelectObject(device_context, font);

    SIZE text_size;
    GetTextExtentPoint32W(device_context, wide_data, wide_length, &text_size);

    SelectObject(device_context, original_font);

    auto label = CreateWindowExW(
        0,
        L"STATIC",
        nullptr,
        WS_CHILD | WS_VISIBLE | ES_LEFT,
        (int)x, (int)y,
        text_size.cx, (int)font_size,
        window,
        nullptr,
        GetModuleHandleA(nullptr),
        nullptr
    );

    SetWindowTextW(label, wide_data);

    free(wide_data);

    SendMessageW(label, WM_SETFONT, (WPARAM)font, TRUE);

    auto index = next_control_index;

    append(&controls, {
        index,
        label
    });

    next_control_index += 1;

    return (control_t*)index;
}

extern "C" control_t *create_button(
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
) {
    wchar_t *wide_data;
    size_t wide_length;
    utf8_to_wide_string(text_data, text_length, &wide_data, &wide_length);

    auto label = CreateWindowExW(
        0,
        L"BUTTON",
        wide_data,
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        (int)x, (int)y,
        (int)width, (int)height,
        window,
        nullptr,
        GetModuleHandleA(nullptr),
        nullptr
    );

    free(wide_data);

    auto font = get_windows_font(font_family_data, font_family_length, (int)font_size);

    SendMessageW(label, WM_SETFONT, (WPARAM)font, TRUE);

    auto index = next_control_index;

    append(&controls, {
        index,
        label
    });

    next_control_index += 1;

    return (control_t*)index;
}

extern "C" control_t *create_text_input(
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
) {
    auto text_input = CreateWindowExW(
        0,
        L"EDIT",
        nullptr,
        WS_CHILD | WS_VISIBLE,
        (int)x, (int)y,
        (int)width, (int)height,
        window,
        nullptr,
        GetModuleHandleA(nullptr),
        nullptr
    );

    wchar_t *wide_data;
    size_t wide_length;
    utf8_to_wide_string(text_data, text_length, &wide_data, &wide_length);

    SetWindowTextW(text_input, wide_data);

    free(wide_data);

    auto font = get_windows_font(font_family_data, font_family_length, (int)font_size);

    SendMessageW(text_input, WM_SETFONT, (WPARAM)font, TRUE);

    auto index = next_control_index;

    append(&controls, {
        index,
        text_input
    });

    next_control_index += 1;

    return (control_t*)index;
}

extern "C" size_t get_text_input_text(control_t *text_input, char *buffer, size_t buffer_size) {
    for(auto control : controls) {
        if(control.index == (size_t)text_input) {
            auto wide_length = GetWindowTextLengthW(control.window);

            auto wide_data = (wchar_t*)malloc((wide_length + 1) * sizeof(wchar_t));

            GetWindowTextW(control.window, wide_data, wide_length + 1);

            char *text_data;
            size_t text_length;
            wide_string_to_utf8(wide_data, &text_data, &text_length);

            if(text_length <= buffer_size) {
                memcpy((void*)buffer, (void*)text_data, text_length);
            }

            free(text_data);
            free(wide_data);

            return text_length;
        }
    }

    abort();
}