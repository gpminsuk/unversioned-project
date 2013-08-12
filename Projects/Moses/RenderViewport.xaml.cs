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

    public class DirectXHost : HwndHost
    {
        public RenderViewport Viewport;
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr CreatedViewport = MosesMain.m_Backend.CreateViewport(Viewport.ViewportType);
            return new HandleRef(this, MosesMain.m_Backend.CreateViewportWindow(CreatedViewport, hwndParent.Handle));
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            MosesMain.m_Backend.RemoveViewport(hwnd.Handle);
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
        private bool IsLoaded = false;

        private void Rectangle_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            e.Handled = mouseMovedAfterRightButtonDown;
        }

        protected override void OnMouseRightButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseRightButtonDown(e);
            mouseRightButtonDown = true;
            mouseMovedAfterRightButtonDown = false;
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            MosesMain.m_Backend.MessageTranslator(GetWindowHandle(), Message.MosesMsg_MouseMove, e);
            if (mouseRightButtonDown)
            {
                mouseMovedAfterRightButtonDown = true;
            }
        }

        protected override void OnMouseWheel(MouseWheelEventArgs e)
        {
            base.OnMouseWheel(e);
            MosesMain.m_Backend.MessageTranslator(GetWindowHandle(), Message.MosesMsg_MouseWheel, e);            
        }

        void OnLoaded(object sender, RoutedEventArgs e)
        {
            if (!IsLoaded)
            {
                DirectXHost Host = new DirectXHost();
                Host.Viewport = this;
                ViewerBorder.Child = Host;
                IsLoaded = true;
            }
        }

        void OnUnloaded(object sender, RoutedEventArgs e)
        {
            if (IsLoaded)
            {
                DirectXHost DXHost = (DirectXHost)ViewerBorder.Child;
                DXHost.Dispose();
                IsLoaded = false;
            }            
        }

        public RenderViewport()
        {
            InitializeComponent();
            
            Unloaded += OnUnloaded;
            Loaded += OnLoaded;
        }

        public IntPtr GetWindowHandle()
        {
            DirectXHost DXHost = (DirectXHost)ViewerBorder.Child;
            return DXHost.Handle;
        }

        public bool Resizing { get; set; }
    }
}
