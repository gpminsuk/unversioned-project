﻿using System;
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
            return new HandleRef(this, MosesMain.m_Backend.CreateMosesWindow(800, 600, hwndParent.Handle));
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            NativeMethods.DestroyWindow(hwnd.Handle);
        }

        protected override void OnWindowPositionChanged(Rect rcBoundingBox)
        {
            MosesMain.m_Backend.ResizeMosesWindow(Handle, (int)rcBoundingBox.Left, (int)rcBoundingBox.Top, (int)rcBoundingBox.Right, (int)rcBoundingBox.Bottom);
        }
    }

    public partial class RenderView : UserControl
    {
        public DirectXHost directXHost;

        private bool mouseRightButtonDown = false;
        private bool mouseMovedAfterRightButtonDown = false;

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

        public RenderView()
        {
            InitializeComponent();

            directXHost = new DirectXHost();
            ViewerBorder.Child = directXHost;
        }
                
        public IntPtr GetWindowHandle()
        {
            return directXHost.Handle;
        }
    }
}
