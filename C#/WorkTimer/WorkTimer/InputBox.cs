using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorkTimer
{
    public partial class InputBox : Form
    {
        public InputBox()
        {
            InitializeComponent();
        }

        private void cmdOk_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void cmdUndo_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        public static InputBoxResult Prompt(string title)
        {
            InputBox ibf = new InputBox();
            InputBoxResult result = new InputBoxResult();
            result.Result = ibf.ShowDialog();
            result.Value = ibf.txtInputText.Text;
            return result;
        }
    }

    public struct InputBoxResult
    {
        public string Value;
        public DialogResult Result;
    }
}
