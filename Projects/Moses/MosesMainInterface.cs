using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Moses
{
    public enum Message
    {
        MosesMsg_MouseMove,
        MosesMsg_MouseWheel,
    }

    public interface IMosesMainInterface
    {
        void Tick(float deltaTime);
        IntPtr CreateViewport(EViewportType ViewportType);
        void RemoveViewport(IntPtr Handle);
        IntPtr CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent);
        void OnViewportsResized();
        void MessageTranslator(IntPtr Handle, Message msg, params object[] list);
    }
}