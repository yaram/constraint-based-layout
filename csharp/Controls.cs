using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using static ConstraintSDK.SystemControls;
using ConstraintSDK.ConstraintArithmetic;

namespace ConstraintSDK.Controls {
    public struct Color {
        public float Red;
        public float Green;
        public float Blue;
        public float Alpha;

        public Color(float red, float green, float blue, float alpha) {
            Red = red;
            Green = green;
            Blue = blue;
            Alpha = alpha;
        }

        public int Pack() {
            return (int)(Red * 0xFF) | (int)(Green * 0xFF) << 8 | (int)(Blue * 0xFF) << 16 | (int)(Alpha * 0xFF) << 24;
        }
    }

    public class Container {
        public class Style {
            public Color BackgroundColor;

            public float BorderSize;
            public Color BorderColor;

            public Style(Color backgroundColor, float borderSize, Color borderColor) {
                BackgroundColor = backgroundColor;
                BorderSize = borderSize;
                BorderColor = borderColor;
            }
        }

        public Container Parent;

        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticVariable Width;
        public ArithmeticVariable Height;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public Style LocalStyle = null;

        public Container.Style DefaultContainerStyle = null;
        public Label.Style DefaultLabelStyle = null;
        public Button.Style DefaultButtonStyle = null;
        public TextInput.Style DefaultTextInputStyle = null;

        public UIntPtr Control = UIntPtr.Zero;

        public Container(LayoutContext context, Container parent) {
            Parent = parent;

            Left = new ArithmeticVariable(context.ArithmeticContext);
            Top = new ArithmeticVariable(context.ArithmeticContext);

            Width = new ArithmeticVariable(context.ArithmeticContext);
            Height = new ArithmeticVariable(context.ArithmeticContext);

            context.Containers.Add(this);
        }
    }

    public class Label {
        public class Style {
            public Color TextColor;

            public Style(Color textColor) {
                TextColor = textColor;
            }
        }

        public Container Container;

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

        public Style LocalStyle = null;

        public UIntPtr Control = UIntPtr.Zero;

        public Label(LayoutContext context, Container container, string text, string fontFamily, float fontSize) {
            Container = container;

            Left = new ArithmeticVariable(context.ArithmeticContext);
            Top = new ArithmeticVariable(context.ArithmeticContext);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;

            context.Labels.Add(this);
        }
    }

    public class Button {
        public class Style {
            public Color TextColor;

            public Color BackgroundColor;

            public float BorderSize;
            public Color BorderColor;

            public Style(Color textColor, Color backgroundColor, float borderSize, Color borderColor) {
                TextColor = textColor;
                BackgroundColor = backgroundColor;
                BorderSize = borderSize;
                BorderColor = borderColor;
            }
        }

        public Container Container;

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

        public Style LocalStyle = null;

        public event Action Press = null;

        public UIntPtr Control = UIntPtr.Zero;

        public Button(LayoutContext context, Container container, string text, string fontFamily, float fontSize) {
            Container = container;

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
        public class Style {
            public Color TextColor;

            public Color BackgroundColor;

            public float BorderSize;
            public Color BorderColor;

            public Style(Color textColor, Color backgroundColor, float borderSize, Color borderColor) {
                TextColor = textColor;
                BackgroundColor = backgroundColor;
                BorderSize = borderSize;
                BorderColor = borderColor;
            }
        }

        public Container Container;

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

        public Style LocalStyle = null;

        public event Action<string> Change = null;

        public UIntPtr Control = UIntPtr.Zero;

        public TextInput(LayoutContext context, Container container, string text, string fontFamily, float fontSize) {
            Container = container;

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

        public List<Container> Containers = new List<Container>();
        public List<Label> Labels = new List<Label>();
        public List<Button> Buttons = new List<Button>();
        public List<TextInput> TextInputs = new List<TextInput>();

        public Color BackgroundColor = new Color(1.0f, 1.0f, 1.0f, 1.0f);

        public Container.Style DefaultContainerStyle = new Container.Style(
            new Color(0.0f, 0.0f, 0.0f, 0.0f),
            0.0f,
            new Color(0.0f, 0.0f, 0.0f, 0.0f)
        );

        public Label.Style DefaultLabelStyle = new Label.Style(
            new Color(0.0f, 0.0f, 0.0f, 1.0f)
        );

        public Button.Style DefaultButtonStyle = new Button.Style(
            new Color(0.0f, 0.0f, 0.0f, 1.0f),
            new Color(0.9f, 0.9f, 0.9f, 1.0f),
            1.0f,
            new Color(0.7f, 0.7f, 0.7f, 1.0f)
        );

        public TextInput.Style DefaultTextInputStyle = new TextInput.Style(
            new Color(0.0f, 0.0f, 0.0f, 1.0f),
            new Color(1.0f, 1.0f, 1.0f, 1.0f),
            1.0f,
            new Color(0.7f, 0.7f, 0.7f, 1.0f)
        );

        public event Action<float, float> FrameResize = null;

        public void PerformLayout() {
            clear_controls(BackgroundColor.Pack());

            ArithmeticContext.Solve();

            

            while(true) {
                var allGenerated = true;

                foreach(var container in Containers) {
                    if(container.Control != UIntPtr.Zero) {
                        continue;
                    }

                    UIntPtr parentControl;
                    if(container.Parent != null) {
                        if(container.Parent.Control == UIntPtr.Zero) {
                            allGenerated = false;

                            continue;
                        }

                        parentControl = container.Parent.Control;
                    } else {
                        parentControl = UIntPtr.Zero;
                    }

                    Container.Style style;
                    if(container.LocalStyle == null) {
                        var parent = container.Parent;

                        while(true) {
                            if(parent != null) {
                                if(parent.DefaultContainerStyle != null) {
                                    style = parent.DefaultContainerStyle;
                                } else if(parent.Parent != null) {
                                    parent = parent.Parent;

                                    continue;
                                } else {
                                    style = DefaultContainerStyle;

                                    break;
                                }
                            } else {
                                style = DefaultContainerStyle;

                                break;
                            }
                        }
                    } else {
                        style = container.LocalStyle;
                    }

                    container.Control = create_container(
                        parentControl,
                        container.Left.Value,
                        container.Top.Value,
                        container.Width.Value,
                        container.Height.Value,
                        style.BackgroundColor.Pack(),
                        style.BorderSize,
                        style.BorderColor.Pack()
                    );
                }

                if(allGenerated) {
                    break;
                }
            }

            foreach(var label in Labels) {
                UIntPtr containerControl;
                if(label.Container != null) {
                    containerControl = label.Container.Control;
                } else {
                    containerControl = UIntPtr.Zero;
                }

                Label.Style style;
                if(label.LocalStyle == null) {
                    var container = label.Container;

                    while(true) {
                        if(container != null) {
                            if(container.DefaultLabelStyle != null) {
                                style = container.DefaultLabelStyle;
                            } else if(container.Parent != null) {
                                container = container.Parent;

                                continue;
                            } else {
                                style = DefaultLabelStyle;

                                break;
                            }
                        } else {
                            style = DefaultLabelStyle;

                            break;
                        }
                    }
                } else {
                    style = label.LocalStyle;
                }

                var textBytes = Encoding.UTF8.GetBytes(label.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(label.FontFamily);

                label.Control = create_label(
                    containerControl,
                    label.Left.Value,
                    label.Top.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    label.FontSize,
                    style.TextColor.Pack()
                );
            }

            foreach(var button in Buttons) {
                UIntPtr containerControl;
                if(button.Container != null) {
                    containerControl = button.Container.Control;
                } else {
                    containerControl = UIntPtr.Zero;
                }

                Button.Style style;
                if(button.LocalStyle == null) {
                    var container = button.Container;

                    while(true) {
                        if(container != null) {
                            if(container.DefaultButtonStyle != null) {
                                style = container.DefaultButtonStyle;
                            } else if(container.Parent != null) {
                                container = container.Parent;

                                continue;
                            } else {
                                style = DefaultButtonStyle;

                                break;
                            }
                        } else {
                            style = DefaultButtonStyle;

                            break;
                        }
                    }
                } else {
                    style = button.LocalStyle;
                }

                var textBytes = Encoding.UTF8.GetBytes(button.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(button.FontFamily);

                button.Control = create_button(
                    containerControl,
                    button.Left.Value,
                    button.Top.Value,
                    button.Width.Value,
                    button.Height.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    button.FontSize,
                    style.TextColor.Pack(),
                    style.BackgroundColor.Pack(),
                    style.BorderSize,
                    style.BorderColor.Pack()
                );
            }

            foreach(var textInput in TextInputs) {
                UIntPtr containerControl;
                if(textInput.Container != null) {
                    containerControl = textInput.Container.Control;
                } else {
                    containerControl = UIntPtr.Zero;
                }

                TextInput.Style style;
                if(textInput.LocalStyle == null) {
                    var container = textInput.Container;

                    while(true) {
                        if(container != null) {
                            if(container.DefaultTextInputStyle != null) {
                                style = container.DefaultTextInputStyle;
                            } else if(container.Parent != null) {
                                container = container.Parent;

                                continue;
                            } else {
                                style = DefaultTextInputStyle;

                                break;
                            }
                        } else {
                            style = DefaultTextInputStyle;

                            break;
                        }
                    }
                } else {
                    style = textInput.LocalStyle;
                }

                var textBytes = Encoding.UTF8.GetBytes(textInput.Text);
                var fontFamilyBytes = Encoding.UTF8.GetBytes(textInput.FontFamily);

                textInput.Control = create_text_input(
                    containerControl,
                    textInput.Left.Value,
                    textInput.Top.Value,
                    textInput.Width.Value,
                    textInput.Height.Value,
                    textBytes,
                    (UIntPtr)textBytes.Length,
                    fontFamilyBytes,
                    (UIntPtr)fontFamilyBytes.Length,
                    textInput.FontSize,
                    style.TextColor.Pack(),
                    style.BackgroundColor.Pack(),
                    style.BorderSize,
                    style.BorderColor.Pack()
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
                if(GlobalLayoutContext != null && GlobalLayoutContext.FrameResize != null) {
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
