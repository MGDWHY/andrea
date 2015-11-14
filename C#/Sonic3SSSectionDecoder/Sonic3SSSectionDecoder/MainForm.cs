using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

namespace Sonic3SSSectionDecoder
{
    public partial class MainForm : Form
    {

        private static readonly uint[] NUM_RINGS = new uint[128] {
            32, 3, 20, 26, 18, 4, 1, 2, 28, 2, 11, 8, 17, 2, 10, 8, 
            64, 11, 20, 36, 3, 8, 12, 10, 54, 18, 1, 1, 52, 1, 164, 
            4, 26, 14, 10, 9, 16, 2, 9, 38, 9, 6, 12, 21, 4, 18, 22, 
            17, 17, 1, 2, 48, 22, 16, 1, 4, 1, 16, 6, 6, 23, 9, 14, 
            3, 26, 38, 30, 24, 40, 5, 12, 45, 12, 3, 12, 6, 20, 3, 
            42, 54, 43, 64, 72, 20, 48, 21, 13, 22, 13, 17, 12, 48, 
            32, 100, 6, 2, 45, 32, 4, 25, 36, 4, 4, 21, 17, 9, 9, 
            2, 9, 1, 16, 18, 11, 5, 36, 33, 9, 40, 5, 48, 2, 9, 1, 
            8, 9, 3, 34, 4
        };

        private static readonly String FILE_FILTER = "Section data files (*.sdf)|*.sdf";
        private static readonly String DATA_FILE_FILTER = "Data files (*.dat)|*.dat";

        private static readonly String STATUS_READY = "Ready";
        private static readonly String STATUS_EXPORTING = "Exporting Data... Please Wait";
        

        private SectionDecoder[] sectionDecoders;

        private static SolidBrush[] dataFontBrushes;
        private static SolidBrush fontBrush;
        private static Font font;


        static MainForm()
        {
            fontBrush = new SolidBrush(Color.White);
            font = new Font("Arial", 10, FontStyle.Bold);
            dataFontBrushes = new SolidBrush[6];

            dataFontBrushes[0] = new SolidBrush(Color.Black);
            dataFontBrushes[1] = new SolidBrush(Color.Red);
            dataFontBrushes[2] = new SolidBrush(Color.Blue);
            dataFontBrushes[3] = new SolidBrush(Color.White);
            dataFontBrushes[4] = new SolidBrush(Color.Yellow);
            dataFontBrushes[5] = new SolidBrush(Color.Orange);
        }

        public MainForm()
        {
            InitializeComponent();

            this.SetStatus(MainForm.STATUS_READY, 0);

            this.LoadSections();

            this.UpdateSectionComboBox();
            
        }

        void LoadSections()
        {
            String[] files = Directory.GetFiles("images\\sections");

            this.sectionDecoders = new SectionDecoder[files.Length];

            for(int i = 0; i < files.Length; i++)
            {
                if (files[i].EndsWith(".png"))
                {
                    this.sectionDecoders[i] = new SectionDecoder(files[i], i);
                    this.sectionDecoders[i].Data.MaxRings = MainForm.NUM_RINGS[i];
                }
            }
        }

        void SetStatus(String text, int progress)
        {
            this.lblStatus.Text = text;
            this.prbStatus.Value = progress;
            if (progress == 0)
                this.prbStatus.Visible = false;
            else
                this.prbStatus.Visible = true;
        }

        void UpdateSectionComboBox()
        {
            this.cmoSelectedSection.Items.Clear();
            for (int i = 0; i < this.sectionDecoders.Length; i++)
            {
                this.cmoSelectedSection.Items.Add(this.sectionDecoders[i]);
            }

            this.cmoSelectedSection.SelectedIndex = 0;
        }

        void UpdateCurrentSection()
        {
            if (this.cmoSelectedSection.SelectedIndex >= 0)
            {
                SectionDecoder current = (SectionDecoder)this.cmoSelectedSection.SelectedItem;

                try
                {
                    current.Data.MaxRings = UInt32.Parse(this.txtMaxRings.Text);
                }
                catch (FormatException ex)
                {
                    MessageBox.Show(this, "Insert non-negative integer value for field Max Rings", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);

                }

            }
        }

        void SaveData(String fileName)
        {
            Stream stream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None);
            IFormatter formatter = new BinaryFormatter();
            try
            {
                formatter.Serialize(stream, this.sectionDecoders.Length);
                foreach (SectionDecoder decoder in this.sectionDecoders)
                    formatter.Serialize(stream, decoder);
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "An error occurred while writing:\n" + ex.Message, 
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            stream.Close();
        }

        void LoadData(String fileName)
        {
            Stream stream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.None);
            IFormatter formatter = new BinaryFormatter();
            try
            {
                this.sectionDecoders = new SectionDecoder[(int)formatter.Deserialize(stream)];
                for(int i = 0; i < this.sectionDecoders.Length; i++)
                    this.sectionDecoders[i] = (SectionDecoder)formatter.Deserialize(stream);

                this.UpdateSectionComboBox();

            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "An error occurred while reading:\n" + ex.Message,
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            stream.Close();
        }

        void ExportData(String fileName)
        {
            Stream stream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None);
            BinaryWriter writer = new BinaryWriter(stream);
            this.SetStatus(MainForm.STATUS_EXPORTING, 0);

            try
            {
                writer.Write((uint)this.sectionDecoders.Length);
                for (int i = 0; i < sectionDecoders.Length; i++)
                {
                    this.SetStatus(MainForm.STATUS_EXPORTING, (int) ((i+1) / (float) sectionDecoders.Length * 100));
                    this.sectionDecoders[i].Decode(true);
                    this.sectionDecoders[i].Data.Write(writer);
                    Application.DoEvents();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "An error occurred while writing:\n" + ex.Message,
                    "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            this.SetStatus(MainForm.STATUS_READY, 0);

            stream.Close();
        }

        #region Event Handlers

        private void cmoSelectedSection_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sender == this.cmoSelectedSection)
            {
                SectionDecoder current =  (SectionDecoder)this.cmoSelectedSection.SelectedItem;
                current.Decode(false);
                this.txtMaxRings.Text = "" + current.Data.MaxRings;
                this.picSectionImage.Invalidate();
            }
        }
        
        private void picSectionImage_Paint(object sender, PaintEventArgs e)
        {
            if (this.cmoSelectedSection.SelectedIndex < 0)
                return;

            float currentY = 0;

            SectionDecoder current = (SectionDecoder)(this.cmoSelectedSection.SelectedItem);

            Graphics g = e.Graphics;

            for (int y = 0; y < SectionDecoder.SECTION_HEIGHT; y++) 
            {
                for (int x = 0; x < SectionDecoder.SECTION_HEIGHT; x++)
                {
                    uint val = current.Data.GetValueAt(x, y);
                    byte avs = current.Data.GetAvoidSearchAt(x, y);

                    if (avs == 1)
                        g.FillRectangle(Brushes.White, x * 20, y * 20, 20, 20);

                    if(val != SectionDecoder.SECTION_ITEM_UNKNOWN)
                        g.DrawString("" + current.Data.GetValueAt(x, y), MainForm.font, MainForm.dataFontBrushes[val], 
                            x * 20, y * 20);
                    else
                        g.DrawString("U", MainForm.font, MainForm.fontBrush, x * 20, y * 20);


                }   
            }

            currentY += SectionDecoder.SECTION_HEIGHT * 20 + 10;

            g.DrawString("Source bitmap", MainForm.font, MainForm.fontBrush, 0, currentY);
            currentY += g.MeasureString("Source bitmap", MainForm.font).Height;

            g.DrawImage(current.SectionBitmap, 0, currentY);
            currentY += current.SectionBitmap.Height + 10;
        }

        private void txtMaxRings_Leave(object sender, EventArgs e)
        {
            this.UpdateCurrentSection();
        }

        private void mnuExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void mnuSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();

            this.UpdateCurrentSection();

            dialog.Filter = MainForm.FILE_FILTER;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.SaveData(dialog.FileName);
            }
        }

        private void mnuOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = MainForm.FILE_FILTER;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.LoadData(dialog.FileName);
            }
        }

        private void mnuExportData_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();

            this.UpdateCurrentSection();

            dialog.Filter = MainForm.DATA_FILE_FILTER;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.ExportData(dialog.FileName);
            }
        }

        private void picSectionImage_MouseDown(object sender, MouseEventArgs e)
        {
            int x = e.X / 20;
            int y = e.Y / 20;

            SectionDecoder current = (SectionDecoder)this.cmoSelectedSection.SelectedItem;

            if (x < 0 || y < 0 || x >= SectionDecoder.SECTION_WIDTH || y >= SectionDecoder.SECTION_HEIGHT)
                return;

            if (current.Data.GetAvoidSearchAt(x, y) == 0)
                current.Data.SetAvoidSearchAt(1, x, y);
            else
                current.Data.SetAvoidSearchAt(0, x, y);

            this.picSectionImage.Invalidate();
        }

        #endregion










    }
}
