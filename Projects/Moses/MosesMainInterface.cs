using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Moses
{
    public enum Message
    {
        MosesMsg_MouseMove,
    }
    public interface IMosesMainInterface
    {
        void Tick(float deltaTime);
        IntPtr CreateMosesWindow(IntPtr hWndParent);
        void MessageTranslator(Message msg, params object[] list);
    }
}