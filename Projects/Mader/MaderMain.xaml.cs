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
        private float m_Scale;
        private Point m_Translation;
        private Point m_PrevMousePos;

        public MaderMain()
        {
            InitializeComponent();

            m_Scale = 1.0f;
            m_Translation = new Point(0, 0);
            m_PrevMousePos = new Point(0, 0);

            m_DirectXHost = new DirectXHost();
            ViewerBorder.Child = m_DirectXHost;
        }

        void ExpressionControl_MouseRightButtonDown(object Sender, MouseButtonEventArgs e)
        {
            m_PrevMousePos = Mouse.GetPosition(ExpressionControl);
        }

        void ExpressionControl_MouseWheel(object Sender, MouseWheelEventArgs e)
        {
            m_Scale *= (1.0f+(e.Delta/120.0f)*0.3f);

            TransformGroup TGroup = new TransformGroup();
            TGroup.Children.Add(new ScaleTransform(m_Scale, m_Scale));
            TGroup.Children.Add(new TranslateTransform(m_Translation.X, m_Translation.Y));
            ExpressionControl.RenderTransform = TGroup;
        }

        void ExpressionControl_MouseMove(object Sender, MouseEventArgs e)
        {
            if (e.RightButton == MouseButtonState.Pressed)
            {
                m_Translation += (m_PrevMousePos - Mouse.GetPosition(ExpressionControl));

                foreach (object Obj in ExpressionControl.Children)
                {
                    if (Obj.GetType() == typeof(MaterialExpressionControl))
                    {
                        ((MaterialExpressionControl)Obj).SetTransform(m_Translation, m_Scale);
                    }
                }
                m_PrevMousePos = Mouse.GetPosition(ExpressionControl);
            }
        }

        public void Initialize(IMaderMainInterface Backend)
        {
            m_Backend = Backend;

            GameLoop = new DispatcherTimer { Interval = TimeSpan.Zero };
            GameLoop.Tick += Idle;
            GameLoop.Start();

            Show();

            m_Backend.CreateMaderApp();

            MaterialExpressionControl Ctrl = new MaterialExpressionControl(5, 3);
            Ctrl.Width = 120;
            Ctrl.Height = 150;
            Ctrl.Margin = new Thickness(0, 0, 0, 0);

            ExpressionControl.Children.Add(Ctrl);
            ExpressionControl.MouseWheel += new MouseWheelEventHandler(ExpressionControl_MouseWheel);
            ExpressionControl.MouseMove += new MouseEventHandler(ExpressionControl_MouseMove);
            ExpressionControl.MouseRightButtonDown += new MouseButtonEventHandler(ExpressionControl_MouseRightButtonDown);
        }

        void Idle(object Sender, EventArgs e)
        {
            m_Backend.Tick();
        }

        public IntPtr GetHwnd()
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
