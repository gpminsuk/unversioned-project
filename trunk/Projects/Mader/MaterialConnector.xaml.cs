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

namespace Mader
{
    /// <summary>
    /// MaterialConnector.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MaterialConnector : UserControl
    {
        public MaterialConnector()
        {
            InitializeComponent();

            MouseEnter += new MouseEventHandler(MaterialConnector_MouseEnter);
            MouseLeave += new MouseEventHandler(MaterialConnector_MouseLeave);
        }

        void MaterialConnector_MouseEnter(object Sender, MouseEventArgs e)
        {
            Connector.Fill = Brushes.LightGray;
        }

        void MaterialConnector_MouseLeave(object Sender, MouseEventArgs e)
        {
            Connector.Fill = Brushes.DarkGray;
        }
    }
}
