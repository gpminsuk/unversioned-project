using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mader
{
    public interface IMaderMainInterface
    {
        void Test();
        bool CreateMaderApp();
        IntPtr HandleMessages(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled);
    }
}
