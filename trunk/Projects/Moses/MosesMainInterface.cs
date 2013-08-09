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
        IntPtr CreateMosesWindow(double Width, double Height, IntPtr hWndParent);
        void ResizeMosesWindow(IntPtr Handle, int left, int top, int right, int bottom);
        void MessageTranslator(IntPtr Handle, Message msg, params object[] list);
    }
}