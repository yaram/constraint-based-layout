using System;
using System.Runtime.InteropServices;

public static class Solver {
    [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.U1)]
    public static extern bool solve(
        UIntPtr variable_count,
        UIntPtr constraint_count,
        [MarshalAs(UnmanagedType.LPArray, ArraySubType=UnmanagedType.U1)] bool[] is_variable_external,
        ref float objective_constant,
        float[] objective_coefficients,
        UIntPtr[] constraint_variable_indices,
        [In, Out] float[] constraint_constants,
        float[] constraint_coefficients
    );
}