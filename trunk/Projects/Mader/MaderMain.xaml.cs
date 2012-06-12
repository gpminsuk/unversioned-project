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

        public MaderMain(IMaderMainInterface Backend)
        {
            m_Backend = Backend;            

            InitializeComponent();

            DispatcherTimer GameLoop = new DispatcherTimer { Interval = TimeSpan.Zero };
            GameLoop.Tick += Idle;
            GameLoop.Start();
            
        }

        public void ShowWindow()
        {
            Show();
        }

        public IntPtr GetWindowHandle()
        {
            return ((HwndSource)HwndSource.FromVisual(this)).Handle;
        }

        void Idle(object Sender, EventArgs e)
        {
            m_Backend.Tick(0);
        }
    }
}
