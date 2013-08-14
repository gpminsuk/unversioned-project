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

namespace Moses
{
    /// <summary>
    /// UserControl1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MosesMain : Window
    {
        static public IMosesMainInterface m_Backend;
        static public MosesMain This;

        public MosesMain(IMosesMainInterface Backend)
        {
            This = this;
            m_Backend = Backend;            

            InitializeComponent();

            DispatcherTimer GameLoop = new DispatcherTimer { Interval = TimeSpan.Zero };
            GameLoop.Tick += Idle;
            GameLoop.Start();
        }

        public ResourceBrowser GetResourceBrowser()
        {
            return ResourceBrowser;
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

        public void AddTab(String Name)
        {
            TabItem Item = new TabItem();
            ClosableHeader header = new ClosableHeader();
            header.button_close.Click += (sender, e) =>
            {
                TabControl.Items.Remove(Item);
                if (Item.Content is ModelView)
                {
                    (Item.Content as ModelView).World.DestroyWorld();
                }
                else if (Item.Content is WorldView)
                {
                    (Item.Content as WorldView).FirstWorld.DestroyWorld();
                }
            };
            Item.Header = header;
            if (Name.EndsWith("exmap"))
            {
                Item.Content = new WorldView();
            }
            else
            {
                Item.Content = new ModelView();
            }
            TabControl.Items.Add(Item);
            Item.IsSelected = true;
        }
    }
}
