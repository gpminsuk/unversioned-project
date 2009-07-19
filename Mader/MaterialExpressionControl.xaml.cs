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
    /// MaterialExpessionControl.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MaterialExpressionControl : UserControl
    {
        public MaterialExpressionControl(int nNumInput, int nNumOutput)
        {
            InitializeComponent();
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
        }

        public void SetTransform(Point Translate, float Scale)
        {
            TransformGroup TGroup = new TransformGroup();
            TGroup.Children.Add(new ScaleTransform(Scale, Scale));
            TGroup.Children.Add(new TranslateTransform(Translate.X, Translate.Y));
            RenderTransform = TGroup;
        }
    }
}
