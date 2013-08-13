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
        MosesMsg_KeyDown,
        MosesMsg_KeyUp,
    }

    public interface IMosesMainInterface
    {
        void Tick(float deltaTime);

        IntPtr LoadObject(IntPtr World, String Path);

        IntPtr CreateWorld(String Name);
        void DestroyWorld(IntPtr World);

        IntPtr CreateViewport(IntPtr World, EViewportType ViewportType);
        void RemoveViewport(IntPtr World, IntPtr Handle);
        void OnViewportsResized(IntPtr World);

        IntPtr CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent);

        void WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled);
        void MessageTranslator(IntPtr Handle, Message msg, params object[] list);
    }
}