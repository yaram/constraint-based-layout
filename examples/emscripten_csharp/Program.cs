using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using ConstraintArithmetic;
using static Controls;

public static class Program {
    public static int Count = 0;
    public static string TestText = "";

    public struct Label {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public float Width { get { return GetTextWidth(Text, FontFamily, FontSize); } }
        public float Height { get { return FontSize; } }

        public string Text;

        public string FontFamily;
        public float FontSize;

        public Label(ArithmeticContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context);
            Top = new ArithmeticVariable(context);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;
        }

        public UIntPtr Solidify() {
            var textBytes = Encoding.UTF8.GetBytes(Text);
            var fontFamilyBytes = Encoding.UTF8.GetBytes(FontFamily);

            return create_label(
                Left.Value,
                Top.Value,
                textBytes,
                (UIntPtr)textBytes.Length,
                fontFamilyBytes,
                (UIntPtr)fontFamilyBytes.Length,
                FontSize
            );
        }
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

    public struct Button {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticVariable Width;
        public ArithmeticVariable Height;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public String Text;

        public String FontFamily;
        public float FontSize;

        public Button(ArithmeticContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context);
            Top = new ArithmeticVariable(context);

            Width = new ArithmeticVariable(context);
            Height = new ArithmeticVariable(context);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;
        }

        public UIntPtr Solidify() {
            var textBytes = Encoding.UTF8.GetBytes(Text);
            var fontFamilyBytes = Encoding.UTF8.GetBytes(FontFamily);

            return create_button(
                Left.Value,
                Top.Value,
                Width.Value,
                Height.Value,
                textBytes,
                (UIntPtr)textBytes.Length,
                fontFamilyBytes,
                (UIntPtr)fontFamilyBytes.Length,
                FontSize
            );
        }
    }

    public struct TextInput {
        public ArithmeticVariable Left;
        public ArithmeticVariable Top;

        public ArithmeticVariable Width;
        public ArithmeticVariable Height;

        public ArithmeticExpression Right { get { return Left + Width; } }
        public ArithmeticExpression Bottom { get { return Top + Height; } }

        public ArithmeticExpression HorizontalMiddle { get { return Left + Width / 2; } }
        public ArithmeticExpression VerticalMiddle { get { return Top + Height / 2; } }

        public String Text;

        public String FontFamily;
        public float FontSize;

        public TextInput(ArithmeticContext context, string text, string fontFamily, float fontSize) {
            Left = new ArithmeticVariable(context);
            Top = new ArithmeticVariable(context);

            Width = new ArithmeticVariable(context);
            Height = new ArithmeticVariable(context);

            Text = text;

            FontFamily = fontFamily;
            FontSize = fontSize;
        }

        public UIntPtr Solidify() {
            var textBytes = Encoding.UTF8.GetBytes(Text);
            var fontFamilyBytes = Encoding.UTF8.GetBytes(FontFamily);

            return create_text_input(
                Left.Value,
                Top.Value,
                Width.Value,
                Height.Value,
                textBytes,
                (UIntPtr)textBytes.Length,
                fontFamilyBytes,
                (UIntPtr)fontFamilyBytes.Length,
                FontSize
            );
        }
    }

    public static void Render() {
        var context = new ArithmeticContext();

        var (frameWidth, frameHeight) = GetFrameSize();

        var testLabel = new Label(context, TestText, "sans-serif", 20);

        var countLabel = new Label(context, Count.ToString(), "sans-serif", 20);
        var countLabelWidth = GetTextWidth(countLabel.Text, countLabel.FontFamily, countLabel.FontSize);

        var incrementButton = new Button(context, "Increment", "sans-serif", 20);
        var incrementButtonTextWidth = GetTextWidth(incrementButton.Text, incrementButton.FontFamily, incrementButton.FontSize);

        var testTextInput = new TextInput(context, TestText, "sans-serif", 20);

        var padding = 8.0f;

        context.AddConstraints(
            testTextInput.Width == frameWidth / 3,
            testTextInput.Height == testTextInput.FontSize + padding * 2,

            testTextInput.HorizontalMiddle == incrementButton.HorizontalMiddle,
            testTextInput.Top == incrementButton.Bottom + padding,

            incrementButton.Width == frameWidth / 2,
            incrementButton.Height == incrementButton.FontSize + padding * 2,

            incrementButton.HorizontalMiddle == frameWidth / 2,
            incrementButton.Top - (incrementButton.Top - countLabel.Bottom) / 2 == frameHeight / 2,

            countLabel.HorizontalMiddle == incrementButton.HorizontalMiddle,
            countLabel.Bottom == incrementButton.Top - padding,

            testLabel.HorizontalMiddle == incrementButton.HorizontalMiddle,
            testLabel.Bottom == countLabel.Top - padding
        );

        context.Solve();

        clear_controls();

        testLabel.Solidify();
        countLabel.Solidify();
        incrementButton.Solidify();
        testTextInput.Solidify();
    }

    public static void Main() {
        try {
            Render();
        } catch(Exception e) {
            Console.Error.WriteLine(e);
        }
    }

    [UnmanagedCallersOnly(EntryPoint = "button_press_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
    public static void button_press_handler(UIntPtr button) {
        try {
            Count += 1;

            Render();
        } catch(Exception e) {
            Console.Error.WriteLine(e);
        }
    }

    [UnmanagedCallersOnly(EntryPoint = "frame_resize_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
    public static void frame_resize_handler() {
        try {
            Render();
        } catch(Exception e) {
            Console.Error.WriteLine(e);
        }
    }

    [UnmanagedCallersOnly(EntryPoint = "text_input_change_handler", CallConvs = new Type[] { typeof(CallConvCdecl) })]
    public static void text_input_change_handler(UIntPtr text_input) { 
        try {   
            var testTextLength = get_text_input_text(text_input, null, (UIntPtr)0);

            var testTextBuffer = new byte[(int)testTextLength];

            get_text_input_text(text_input, testTextBuffer, testTextLength);

            TestText = Encoding.UTF8.GetString(testTextBuffer, 0, (int)testTextLength);

            Render();
        } catch(Exception e) {
            Console.Error.WriteLine(e);
        }
    }
}