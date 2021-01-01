using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using static ConstraintSDK.SystemControls;
using ConstraintSDK.ConstraintArithmetic;

namespace ConstraintSDK.Controls {
    public class Label {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public float Width { get { return LayoutContext.GetTextWidth(Text, FontFamily, FontSize); } }
        public float Height { get { return FontSize; } }

        public string Text;

        public string FontFamily;
        public float FontSize;

        public UIntPtr Control = UIntPtr.Zero;

        public Label(LayoutContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context.ArithmeticContext);
            Top = new ArithmeticVariable(context.ArithmeticContext);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;

            context.Labels.Add(this);
        }
    }

    public class Button {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticVariable Width;
        public ArithmeticVariable Height;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public string Text;

        public string FontFamily;
        public float FontSize;

        public event Action Press = null;

        public UIntPtr Control = UIntPtr.Zero;

        public Button(LayoutContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context.ArithmeticContext);
            Top = new ArithmeticVariable(context.ArithmeticContext);

            Width = new ArithmeticVariable(context.ArithmeticContext);
            Height = new ArithmeticVariable(context.ArithmeticContext);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;

            context.Buttons.Add(this);
        }

        public void OnPress() {
            if(Press != null) {
                Press();
            }
        }
    }

    public class TextInput {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticVariable Width;
        public ArithmeticVariable Height;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public string Text;

        public string FontFamily;
        public float FontSize;

        public event Action<string> Change = null;

        public UIntPtr Control = UIntPtr.Zero;

        public TextInput(LayoutContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context.ArithmeticContext);
            Top = new ArithmeticVariable(context.ArithmeticContext);

            Width = new ArithmeticVariable(context.ArithmeticContext);
            Height = new ArithmeticVariable(context.ArithmeticContext);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;

            context.TextInputs.Add(this);
        }

        public void OnChange() {
            if(Change != null) {
                var textLength = get_text_input_text(Control, null, (UIntPtr)0);

                var textBuffer = new byte[(int)textLength];

                get_text_input_text(Control, textBuffer, textLength);

                var text = Encoding.UTF8.GetString(textBuffer, 0, (int)textLength);

                Change(text);
            }
        }
    }

    public class LayoutContext {
        public static LayoutContext GlobalLayoutContext;

        public ArithmeticContext ArithmeticContext = new ArithmeticContext();

        public List<Label> Labels = new List<Label>();
        public List<Button> Buttons = new List<Button>();
        public List<TextInput> TextInputs = new List<TextInput>();

        public event Action<float, float> FrameResize = null;

        public void PerformLayout() {
            clear_controls();

            ArithmeticContext.Solve();

            foreach(var label in Labels) {
                var textBytes = Encoding.UTF8.GetBytes(label.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(label.FontFamily);

                label.Control = create_label(
                    label.Left.Value,
                    label.Top.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    label.FontSize
                );
            }

            foreach(var button in Buttons) {
                var textBytes = Encoding.UTF8.GetBytes(button.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(button.FontFamily);

                button.Control = create_button(
                    button.Left.Value,
                    button.Top.Value,
                    button.Width.Value,
                    button.Height.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    button.FontSize
                );
            }

            foreach(var textInput in TextInputs) {
                var textBytes = Encoding.UTF8.GetBytes(textInput.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(textInput.FontFamily);

                textInput.Control = create_text_input(
                    textInput.Left.Value,
                    textInput.Top.Value,
                    textInput.Width.Value,
                    textInput.Height.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    textInput.FontSize
                );
            }

            GlobalLayoutContext = this;
        }

        public void AddConstraints(params ArithmeticConstraint[] constraints) {
            ArithmeticContext.AddConstraints(constraints);
        }

        public static float GetTextWidth(string text, string fontFamily, float fontSize) {
            var textBytes = Encoding.UTF8.GetBytes(text);
            var fontFamilyBytes = Encoding.UTF8.GetBytes(fontFamily);

            return get_text_width(
                textBytes,
                (UIntPtr)textBytes.Length,
                fontFamilyBytes,
                (UIntPtr)fontFamilyBytes.Length,
                fontSize
            );
        }

        public static (float width, float height) GetFrameSize() {
            float width;
            float height;
            get_frame_size(out width, out height);

            return (width, height);
        }

        [UnmanagedCallersOnly(EntryPoint = "button_press_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
        public static void button_press_handler(UIntPtr button) {
            try {
                foreach(var savedButton in GlobalLayoutContext.Buttons) {
                    if(savedButton.Control == button) {
                        savedButton.OnPress();

                        break;
                    }
                }
            } catch(Exception e) {
                Console.Error.WriteLine(e);
            }
        }

        [UnmanagedCallersOnly(EntryPoint = "frame_resize_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
        public static void frame_resize_handler() {
            try {
                if(GlobalLayoutContext.FrameResize != null) {
                    float width;
                    float height;
                    get_frame_size(out width, out height);

                    GlobalLayoutContext.FrameResize(width, height);
                }
            } catch(Exception e) {
                Console.Error.WriteLine(e);
            }
        }

        [UnmanagedCallersOnly(EntryPoint = "text_input_change_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
        public static void text_input_change_handler(UIntPtr text_input) { 
            try {   
                foreach(var savedTextInput in GlobalLayoutContext.TextInputs) {
                    if(savedTextInput.Control == text_input) {
                        savedTextInput.OnChange();

                        break;
                    }
                }
            } catch(Exception e) {
                Console.Error.WriteLine(e);
            }
        }
    }
}
