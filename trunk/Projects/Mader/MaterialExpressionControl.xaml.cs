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
using System.Security.Permissions;

using System.Windows.Threading;

namespace Mader
{
    /// <summary>
    /// MaterialExpessionControl.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MaterialExpressionControl : UserControl
    {
        public MaterialExpressionControl(int nNumInput, int nNumOutput)
        {
            InitializeComponent();

            CompositionTarget.Rendering += new EventHandler(CompositionTarget_Rendering);

            HorizontalAlignment = HorizontalAlignment.Left;
            VerticalAlignment = VerticalAlignment.Top;
            ExpressionName.Content = "ExpresionName";
            for (int i = 0; i < nNumInput; ++i)
            {
                MaterialConnector Connector = new MaterialConnector();
                Connector.Margin = new Thickness(2, 35 + i * 11, 0, 0);
                Connector.HorizontalAlignment = HorizontalAlignment.Left;                
                Connector.VerticalAlignment = VerticalAlignment.Top;
                MaterialExpressionGrid.Children.Add(Connector);
            }
            for (int i = 0; i < nNumOutput; ++i)
            {
                MaterialConnector Connector = new MaterialConnector();
                Connector.Margin = new Thickness(0, 35 + i * 11, 2, 0);
                Connector.HorizontalAlignment = HorizontalAlignment.Right;
                Connector.VerticalAlignment = VerticalAlignment.Top;
                MaterialExpressionGrid.Children.Add(Connector);
            }
            //PreviewImage.Lock();
            //PreviewImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, MaderMain.m_Backend.GetBackBuffer());
            //PreviewImage.AddDirtyRect(new Int32Rect(0, 0, PreviewImage.PixelWidth, PreviewImage.PixelHeight));
            //PreviewImage.Unlock();
        }

        public void CompositionTarget_Rendering(object Sender, EventArgs e)
        {
            /*RenderingEventArgs args = (RenderingEventArgs)e;

            // It's possible for Rendering to call back twice in the same frame 
            // so only render when we haven't already rendered in this frame.
            if (PreviewImage.IsFrontBufferAvailable && _lastRender != args.RenderingTime)
            {
                IntPtr pSurface = IntPtr.Zero;
                HRESULT.Check(GetBackBufferNoRef(out pSurface));
                if (pSurface != IntPtr.Zero)
                {
                    PreviewImage.Lock();
                    // Repeatedly calling SetBackBuffer with the same IntPtr is 
                    // a no-op. There is no performance penalty.
                    PreviewImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, pSurface);
                    HRESULT.Check(Render());
                    PreviewImage.AddDirtyRect(new Int32Rect(0, 0, PreviewImage.PixelWidth, PreviewImage.PixelHeight));
                    PreviewImage.Unlock();

                    _lastRender = args.RenderingTime;
                }
            }*/
        }

        public void SetTransform(Point Translate, float Scale)
        {
            TransformGroup TGroup = new TransformGroup();
            TGroup.Children.Add(new ScaleTransform(Scale, Scale));
            TGroup.Children.Add(new TranslateTransform(Translate.X, Translate.Y));
            RenderTransform = TGroup;
        }
    }

    public static class HRESULT
    {
        [SecurityPermissionAttribute(SecurityAction.Demand, Flags = SecurityPermissionFlag.UnmanagedCode)]
        public static void Check(int hr)
        {
            Marshal.ThrowExceptionForHR(hr);
        }
    }
}
