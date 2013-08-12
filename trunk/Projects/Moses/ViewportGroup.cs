using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;

namespace Moses
{
    public partial class ViewportGroup : Grid
    {
        public ViewportGroup()
        {
            LayoutUpdated += OnLayoutUpdated;
        }

        private void OnLayoutUpdated(object sender, EventArgs e)
        {
            MosesMain.m_Backend.OnViewportsResized();
        }
    }
}
