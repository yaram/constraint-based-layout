using System;
using System.Text;

public static class Program {
    public static void Main() {
        try {
            Console.WriteLine("Test");

            var text = "Best Test";
            var textBytes = Encoding.UTF8.GetBytes(text);

            var fontFamily = "sans-serif";
            var fontFamilyBytes = Encoding.UTF8.GetBytes(fontFamily);

            var label = Controls.create_label(
                10.0f,
                10.0f,
                textBytes,
                (UIntPtr)textBytes.Length,
                fontFamilyBytes,
                (UIntPtr)fontFamilyBytes.Length,
                20.0f
            );
        } catch(Exception e) {
            Console.WriteLine(e);
        }
    }
}