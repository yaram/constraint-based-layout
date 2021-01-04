using System;
using System.Runtime.InteropServices;

namespace ConstraintSDK {
    public static class SystemControls {
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
        public static extern void clear_controls(int background_color);

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr create_container(
            UIntPtr parent,
            float x,
            float y,
            float width,
            float height,
            int background_color,
            float border_size,
            int border_color
        );

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr create_label(
            UIntPtr container,
            float x,
            float y,
            byte[] text_data,
            UIntPtr text_length,
            byte[] font_family_data,
            UIntPtr font_family_length,
            float font_size,
            int text_color
        );

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr create_button(
            UIntPtr container,
            float x,
            float y,
            float width,
            float height,
            byte[] text_data,
            UIntPtr text_length,
            byte[] font_family_data,
            UIntPtr font_family_length,
            float font_size,
            int text_color,
            int background_color,
            float border_size,
            int border_color
        );

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr create_text_input(
            UIntPtr container,
            float x,
            float y,
            float width,
            float height,
            byte[] text_data,
            UIntPtr text_length,
            byte[] font_family_data,
            UIntPtr font_family_length,
            float font_size,
            int text_color,
            int background_color,
            float border_size,
            int border_color
        );

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern UIntPtr get_text_input_text(
            UIntPtr text_input,
            [In, Out] byte[] buffer,
            UIntPtr buffer_size
        );
    }
}