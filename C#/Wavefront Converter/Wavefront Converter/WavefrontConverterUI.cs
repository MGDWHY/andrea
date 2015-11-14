using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WavefrontConverter
{
    public partial class WavefrontConverterUI : Form
    {

        private WFObject current;
        private OpenFileDialog openDialog;
        private SaveFileDialog saveDialog;
        private Dictionary<string, WFFileConverter> converters;

        public WavefrontConverterUI()
        {
            InitializeComponent();
            current = null;

            openDialog = new OpenFileDialog();
            saveDialog = new SaveFileDialog();

            converters = new Dictionary<string, WFFileConverter>();

            Android3DObjectConverter a3oConverter = new Android3DObjectConverter();

            converters.Add(a3oConverter.FormatName, a3oConverter);
            
            foreach(WFFileConverter c in converters.Values) 
            {
                ToolStripMenuItem item = new ToolStripMenuItem(c.FormatName);
                item.Click += new EventHandler(ExportClick);
                mnuExportFile.DropDown.Items.Add(item);
            }
        }

        private void ExportClick(object sender, EventArgs e)
        {
            string converterName = ((ToolStripMenuItem)sender).Text;
            WFFileConverter converter = null;

            if (current == null)
            {
                MessageBox.Show("Open a wavefront file to export");
                return;
            }

            if (converters.TryGetValue(converterName, out converter))
            {
                saveDialog.Filter = converter.FormatName + "(*." + converter.FileExtension + ")|*." + converter.FileExtension;
                if (saveDialog.ShowDialog() == DialogResult.OK)
                {
                    FileStream outStream = File.Open(saveDialog.FileName, FileMode.Create);
                    converter.Write(current, outStream);
                }
            }
        }

        private void loadFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openDialog.Filter = "Wavefront files (*.obj)|*.obj";
            if (openDialog.ShowDialog() == DialogResult.OK)
            {
                current = WFObject.Load(openDialog.FileName);
                wbLog.DocumentText = current.LoadLog;
            }
        }
    }
}
