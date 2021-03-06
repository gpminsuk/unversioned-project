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

namespace Moses
{
    /// <summary>
    /// Interaction logic for WorldView.xaml
    /// </summary>
    public partial class WorldView : UserControl
    {
        public WorldView()
        {
            InitializeComponent();
        }

        private void MenuItemClick_AddThing(object sender, RoutedEventArgs e)
        {
            IntPtr Asset = MosesMain.m_Backend.LoadAsset(((sender as MenuItem).Header as string).Substring(4));
            ThingCreator.ThingCreatorMain Main = new ThingCreator.ThingCreatorMain(Asset, "CMeshPrimitive", "CCharacter");
            bool? result = Main.ShowDialog();
            if (result.Value)
            {
                MosesMain.m_Backend.AddToWorld(FirstWorld.pWorld, Main.Thing, Asset);
            }
        }
    }
}
