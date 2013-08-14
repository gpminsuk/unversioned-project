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
        IntPtr LoadAsset(String Path);

        IntPtr CreateWorld(String Name);
        void DestroyWorld(IntPtr World);

        IntPtr CreateViewport(IntPtr World, EViewportType ViewportType);
        void RemoveViewport(IntPtr World, IntPtr Handle);
        void OnViewportsResized(IntPtr World);

        IntPtr CreateViewportWindow(IntPtr Viewport, IntPtr hWndParent);

        void SetMousePosition(double X, double Y);

        void MessageTranslator(IntPtr Handle, Message msg, params object[] list);

        void AddToWorld(IntPtr World, IntPtr Thing, IntPtr Asset);
        IntPtr CreateThing(String PrimitiveClassName, String ThingClassName);
        String[] CreatablePrimitiveClassNames(IntPtr Asset);
        String[] CreatableThingClassNames(IntPtr Primitive);
        String[] GetNeededAssetClassNames(IntPtr Asset);
    }
}