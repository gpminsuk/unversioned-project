using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mader
{
    public interface IMaderMainInterface
    {
        void Test();
        void Tick();
        bool CreateMaderApp();
        IntPtr GetBackBuffer();
        IntPtr HandleMessages(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled);
    }
}
