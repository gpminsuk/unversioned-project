using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace Mader
{
    public partial class NativeConsts
    {
        internal const int
          WS_CHILD = 0x40000000,
          WS_VISIBLE = 0x10000000,
          LBS_NOTIFY = 0x00000001,
          HOST_ID = 0x00000002,
          LISCBoxPrimitive_ID = 0x00000001,
          WS_VSCROLL = 0x00200000,
          WS_BORDER = 0x00800000;
    }

    public partial class NativeMethods
    {    
        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(int dwExStyle,
                                                      string lpszClassName,
                                                      string lpszWindowName,
                                                      int style,
                                                      int x, int y,
                                                      int width, int height,
                                                      IntPtr hwndParent,
                                                      IntPtr hMenu,
                                                      IntPtr hInst,
                                                      [MarshalAs(UnmanagedType.AsAny)] object pvParam);
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);   

    }

    class DirectXHost : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {    
            IntPtr hwndStatic = NativeMethods.CreateWindowEx(0, "static", "",
                            NativeConsts.WS_CHILD | NativeConsts.WS_VISIBLE,
                            0, 0,
                            0, 0,
                            hwndParent.Handle,
                            (IntPtr)NativeConsts.HOST_ID,
                            IntPtr.Zero,
                            0);

            return new HandleRef(null, hwndStatic);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            NativeMethods.DestroyWindow(hwnd.Handle);
        }
    }
}
