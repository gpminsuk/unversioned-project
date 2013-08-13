using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace Moses
{
    /// <summary>
    /// Interaction logic for RenderViewport.xaml
    /// </summary>
    public partial class NativeMethods
    {
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
    }

    class DXHwndHost : HwndHost
    {
        public DXHwndHost()
        {
            Focusable = true;
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            FrameworkElement IterWorld = (FrameworkElement)Parent;
            while (!(IterWorld is World)) { IterWorld = (FrameworkElement)IterWorld.Parent; }
            FrameworkElement IterRenderViewport = (FrameworkElement)Parent;
            while (!(IterRenderViewport is RenderViewport)) { IterRenderViewport = (FrameworkElement)IterRenderViewport.Parent; }
            IntPtr CreatedViewport = MosesMain.m_Backend.CreateViewport((IterWorld as World).pWorld, (IterRenderViewport as RenderViewport).ViewportType);
            return new HandleRef(this, MosesMain.m_Backend.CreateViewportWindow(CreatedViewport, hwndParent.Handle));
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            FrameworkElement IterWorld = (FrameworkElement)Parent;
            while (!(IterWorld is World)) { IterWorld = (FrameworkElement)IterWorld.Parent; }
            if (((World)IterWorld).pWorld.ToInt32() != 0)
            {
                MosesMain.m_Backend.RemoveViewport(((World)IterWorld).pWorld, hwnd.Handle);
            }
            NativeMethods.DestroyWindow(hwnd.Handle);
        }
    }

    public enum EViewportType
    {
        Perspective,
        Top,
        Bottom,
        Right,
        Left,
        Front,
        Back
    };

    public partial class RenderViewport : UserControl
    {
        public EViewportType ViewportType { get; set; }

        private bool mouseRightButtonDown = false;
        private bool mouseMovedAfterRightButtonDown = false;
        private bool IsLoadedCalled = false;

        private void Rectangle_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            e.Handled = mouseMovedAfterRightButtonDown;
        }

        private void Rectangle_ContextMenuClosing(object sender, ContextMenuEventArgs e)
        {
            e.Handled = mouseMovedAfterRightButtonDown;
        }        

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseRightButtonDown(e);
            mouseRightButtonDown = true;
            mouseMovedAfterRightButtonDown = false;
        }

        void OnLoaded(object sender, RoutedEventArgs e)
        {
            if (!IsLoadedCalled)
            {
                DXCanvas.Child = new DXHwndHost();
                IsLoadedCalled = true;
            }
        }

        void OnUnloaded(object sender, RoutedEventArgs e)
        {
            if (IsLoadedCalled)
            {
                (DXCanvas.Child as DXHwndHost).Dispose();
                IsLoadedCalled = false;
            }
        }

        IntPtr GetHandle()
        {
            return (DXCanvas.Child as DXHwndHost).Handle;
        }

        public RenderViewport()
        {
            InitializeComponent();
            
            Unloaded += OnUnloaded;
            Loaded += OnLoaded;

            Focusable = true;
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            base.OnMouseDown(e);
            Focus();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            MosesMain.m_Backend.MessageTranslator(GetHandle(), Message.MosesMsg_MouseMove, e);
            if (mouseRightButtonDown)
            {
                mouseMovedAfterRightButtonDown = true;
            }
        }

        protected override void OnMouseWheel(MouseWheelEventArgs e)
        {
            base.OnMouseWheel(e);
            MosesMain.m_Backend.MessageTranslator(GetHandle(), Message.MosesMsg_MouseWheel, e);
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);
            MosesMain.m_Backend.MessageTranslator(GetHandle(), Message.MosesMsg_KeyUp, e);
        }

        protected override void OnPreviewKeyDown(KeyEventArgs e)
        {
            base.OnPreviewKeyDown(e);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            MosesMain.m_Backend.MessageTranslator(GetHandle(), Message.MosesMsg_KeyDown, e);
        }
    }
}
