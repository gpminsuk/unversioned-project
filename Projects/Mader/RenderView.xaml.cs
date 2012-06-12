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

namespace Mader
{
    /// <summary>
    /// RenderView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class NativeMethods
    {
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
    }

    public class DirectXHost : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            return new HandleRef(this, MaderMain.m_Backend.CreateMaderWindow(hwndParent.Handle));
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            NativeMethods.DestroyWindow(hwnd.Handle);
        }
    }

    public partial class RenderView : UserControl
    {
        public DirectXHost m_DirectXHost;

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            MaderMain.m_Backend.MessageTranslator(Message.MaderMsg_MouseMove, e);
        }

        public RenderView()
        {
            InitializeComponent();

            m_DirectXHost = new DirectXHost();
            ViewerBorder.Child = m_DirectXHost;
        }
                
        public IntPtr GetWindowHandle()
        {
            return m_DirectXHost.Handle;
        }
    }
}
