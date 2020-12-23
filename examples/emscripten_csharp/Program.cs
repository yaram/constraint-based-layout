using System;

public static class Program {
    public static void Main() {
        try {
            Console.WriteLine("Test");

            const int variableCount = 2;
            const int constraintCount = 1;
            var isVariableExternal = new bool[variableCount] { true, false };
            var objectiveConstant = 0.0f;
            var objectiveCoefficients = new float[variableCount] { 0.0f, 0.0f };
            var constraintVariableIndices = new UIntPtr[constraintCount];
            var constraintConstants = new float[constraintCount] { 0.0f };
            var constraintCoefficients = new float[constraintCount * variableCount] { 1.0f, 1.0f };

            var result = Solver.solve(
                (UIntPtr)variableCount,
                (UIntPtr)constraintCount,
                isVariableExternal,
                ref objectiveConstant,
                objectiveCoefficients,
                constraintVariableIndices,
                constraintConstants,
                constraintCoefficients
            );

            if(result) {
                foreach(var constant in constraintConstants) {
                    Console.WriteLine(constant);
                }
            } else {
                Console.WriteLine("Objective is unbounded");
            }
        } catch(Exception e) {
            Console.WriteLine(e);
        }
    }
}