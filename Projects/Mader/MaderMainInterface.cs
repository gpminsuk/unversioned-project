using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mader
{
    public interface IMaderMainInterface
    {
        void Tick(float deltaTime);
        IntPtr CreateMaderWindow(IntPtr hWndParent);
    }
}
