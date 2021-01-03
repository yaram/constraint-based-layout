using System;
using System.Runtime.InteropServices;

namespace ConstraintSDK {
    public static class SystemControlsWin32 {
        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        public static extern void init_system_controls();

        [DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool wait_for_events();
    }
}