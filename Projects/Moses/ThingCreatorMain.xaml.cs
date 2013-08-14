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

namespace ThingCreator
{
    /// <summary>
    /// ThingCreatorMain.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ThingCreatorMain : Window
    {
        public IntPtr Thing;
        public ThingCreatorMain(IntPtr Asset, String PrimitiveName, String ThingName)
        {
            InitializeComponent();

            Thing = Moses.MosesMain.m_Backend.CreateThing(PrimitiveName, ThingName);
            String[] names = Moses.MosesMain.m_Backend.GetNeededAssetClassNames(Thing);
            for (int i = 0; i < names.Length; ++i)
            {
                Grid.RowDefinitions.Add(new RowDefinition() { Height = GridLength.Auto });
                TextBox Text = new TextBox();
                TextBox Value = new TextBox();
                Text.Text = names[i];
                Value.Text = "..\\..\\Resources\\Renekton_brutal.exskn";
                Grid.Children.Add(Text);
                Grid.Children.Add(Value);
                Grid.SetColumn(Text, 0);
                Grid.SetColumn(Value, 1);
                Grid.SetRow(Text, i);
                Grid.SetRow(Value, i);
            }            
        }

        private void OnClick_Create(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }

        private void OnClick_Cancel(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
