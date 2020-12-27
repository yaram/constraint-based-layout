using System;
using System.Runtime.InteropServices;

public static class Controls {
    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern float get_text_width(
        byte[] text_data,
        UIntPtr text_length,
        byte[] font_family_data,
        UIntPtr font_family_length,
        float font_size
    );

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern void get_frame_size(
        out float width,
        out float height
    );

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern void clear_controls();

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr create_label(
        float x,
        float y,
        byte[] text_data,
        UIntPtr text_length,
        byte[] font_family_data,
        UIntPtr font_family_length,
        float font_size
    );

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr create_button(
        float x,
        float y,
        float width,
        float height,
        byte[] text_data,
        UIntPtr text_length,
        byte[] font_family_data,
        UIntPtr font_family_length,
        float font_size
    );

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr create_text_input(
        float x,
        float y,
        float width,
        float height,
        byte[] text_data,
        UIntPtr text_length,
        byte[] font_family_data,
        UIntPtr font_family_length,
        float font_size
    );

    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    public static extern UIntPtr get_text_input_text(
        UIntPtr text_input,
        [In, Out] byte[] buffer,
        UIntPtr buffer_size
    );
}