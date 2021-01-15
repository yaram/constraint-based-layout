using System;
using ConstraintSDK.Controls;

public static class Program {
    public static int Count = 0;
    public static string TestText = "";

    public static float FrameWidth;
    public static float FrameHeight;

    public static void Render() {
        var context = new LayoutContext();

        context.FrameResize += (width, height) => {
            FrameWidth = width;
            FrameHeight = height;

            Render();
        };

        var content = new Container(context, null);

        var testLabel = new Label(context, content, TestText, "sans-serif", 20);

        var countLabel = new Label(context, content, Count.ToString(), "sans-serif", 20);
        var countLabelWidth = LayoutContext.GetTextWidth(countLabel.Text, countLabel.FontFamily, countLabel.FontSize);

        var incrementButton = new Button(context, content, "Increment", "sans-serif", 20);
        var incrementButtonTextWidth = LayoutContext.GetTextWidth(incrementButton.Text, incrementButton.FontFamily, incrementButton.FontSize);

        incrementButton.Press += () => {
            Count += 1;

            Render();
        };

        var testTextInput = new TextInput(context, content, TestText, "sans-serif", 20);

        testTextInput.Change += text => {
            TestText = text;

            Render();
        };

        var padding = 8.0f;

        context.AddConstraints(
            content.HorizontalMiddle == FrameWidth / 2,
            content.VerticalMiddle == FrameHeight / 2,

            testLabel.Top == 0,

            testTextInput.Width == FrameWidth / 3,
            testTextInput.Height == testTextInput.FontSize+ padding * 2,

            testTextInput.HorizontalMiddle == content.Width / 2,
            testTextInput.Top == incrementButton.Bottom + padding,

            incrementButton.Width == FrameWidth / 2,
            incrementButton.Height == incrementButton.FontSize + padding * 2,

            incrementButton.HorizontalMiddle == content.Width / 2,

            countLabel.HorizontalMiddle == content.Width / 2,
            countLabel.Bottom == incrementButton.Top - padding,

            testLabel.HorizontalMiddle == content.Width / 2,
            testLabel.Bottom == countLabel.Top - padding
        );

        context.PerformLayout();
    }

    public static void Main() {
        try {
            (FrameWidth, FrameHeight) = LayoutContext.GetFrameSize();

            Render();
        } catch(Exception e) {
            Console.Error.WriteLine(e);
        }
    }
}