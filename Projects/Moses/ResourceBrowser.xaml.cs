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
using System.IO;

namespace Moses
{
    /// <summary>
    /// Interaction logic for ResourceBrowser.xaml
    /// </summary>    

    public partial class ResourceBrowser : UserControl
    {
        public ResourceBrowser()
        {
            InitializeComponent();

            string[] files = Directory.GetFiles("..\\..\\Resources\\");
            for (int i = 0; i < files.Length; ++i)
            {
                TreeViewItem Item = new TreeViewItem();
                Item.Header = files[i];
                TreeView.Items.Add(Item);
                Item.MouseDoubleClick += (sender, e) => {
                    MosesMain.This.AddTab(Item.Header.ToString());
                    MosesMain.m_Backend.LoadObject((MosesMain.This.TabControl.SelectedContent as ModelView).World.pWorld, Item.Header as string);
                };
            }            
        }
    }
}
