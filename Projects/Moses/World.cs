using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows;

namespace Moses
{
    public partial class World : Grid
    {
        public IntPtr pWorld;
        public String WorldName { get; set; }
        public bool ShouldCreateWorld { get; set; }

        public World()
        {
            LayoutUpdated += OnLayoutUpdated;
        }

        protected override void OnInitialized(EventArgs e)
        {
            if (ShouldCreateWorld)
            {
                pWorld = MosesMain.m_Backend.CreateWorld(WorldName);
            }
            base.OnInitialized(e);
        }

        public void DestroyWorld()
        {
            MosesMain.m_Backend.DestroyWorld(pWorld);
            pWorld = new IntPtr(0);
        }

        private void OnLayoutUpdated(object sender, EventArgs e)
        {
            if (pWorld.ToInt32() != 0)
            {
                MosesMain.m_Backend.OnViewportsResized(pWorld);                
            }
        }
    }
}
