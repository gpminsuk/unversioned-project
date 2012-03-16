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
using System.Windows.Threading;
using System.Collections;

namespace Mader
{
    /// <summary>
    /// UserControl1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MaderMain : Window
    {
        static public IMaderMainInterface m_Backend;
        private DirectXHost m_DirectXHost;
        private DispatcherTimer GameLoop;

        public MaderMain()
        {
            InitializeComponent();

            m_DirectXHost = new DirectXHost();
            ViewerBorder.Child = m_DirectXHost;
        }

        public void Initialize(IMaderMainInterface Backend)
        {
            m_Backend = Backend;

            GameLoop = new DispatcherTimer { Interval = TimeSpan.Zero };
            GameLoop.Tick += Idle;
            GameLoop.Start();

            Show();

            m_Backend.CreateMaderApp();
        }

        void Idle(object Sender, EventArgs e)
        {
            m_Backend.Tick();
        }

        public IntPtr GetWindowHandle()
        {
            return new WindowInteropHelper(this).Handle;
        }

        public void GetRendererWindowSize(ref int width, ref int height)
        {
            width = 8000;
            height = 600;
        }

        public IntPtr GetRendererWindowHandle()
        {
            return m_DirectXHost.Handle;
        }

        public void SetHwndSourceHook()
        {
            HwndSource Source = (HwndSource)PresentationSource.FromVisual(this);
            Source.AddHook(m_Backend.HandleMessages);
        }
    }
}
