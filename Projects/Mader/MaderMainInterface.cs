using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Mader
{
    public enum Message
    {
        MaderMsg_MouseMove,
    }
    public interface IMaderMainInterface
    {
        void Tick(float deltaTime);
        IntPtr CreateMaderWindow(IntPtr hWndParent);
        void MessageTranslator(Message msg, params object[] list);
    }
}