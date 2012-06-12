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
using System.IO;
using System.Windows.Interop;

namespace ResourceManager
{
    /// <summary>
    /// ResourceManagerMain.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ResourceManagerMain : Window
    {
        public ResourceManagerMain()
        {
            InitializeComponent();

            string[] files = Directory.GetFiles("..\\..\\Resources\\");
            for( int i=0; i<files.Length; ++i )
            {
                resourceView.AddResource(files[i]);
            }
        }
    }
}
