using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BlueSpheresForeverEditor
{
    public partial class AboutForm : Form
    {
        public AboutForm()
        {
            InitializeComponent();

            this.VersionLabel.Text = "Version " + Application.ProductVersion;
        }

        private void Close(object sender, EventArgs e)
        {
            this.Dispose();
        }

    }
}
