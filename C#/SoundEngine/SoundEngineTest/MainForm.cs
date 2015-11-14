using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SoundEngine;
using System.IO;

namespace SoundEngineTest
{
    public partial class MainForm : Form
    {
        SoundStream current = null;

        public MainForm()
        {
            InitializeComponent();
            SoundManager.Initialize();
            UpdateControls();
        }

        private void OpenFile(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Audio files (*.mp3, *.wav, *.ogg)|*.mp3;*.wav;*.ogg";

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                try
                {
                    current = SoundManager.FromFile(dialog.FileName, true);
                    lblFileName.Text = Path.GetFileName(dialog.FileName);
                    UpdateControls();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(this, ex.Message, "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    current = null;
                }
            }
        }

        private void UpdateSound(object sender, EventArgs e)
        {
            if (current != null)
            {
                current.Tempo = tbTempo.Value;
                current.Volume = tbVolume.Value;

                UpdateControls();
            }
        }

        private void UpdateControls()
        {
            if (current != null)
            {
                tbTempo.Enabled = true;
                tbVolume.Enabled = true;
                cmdPlay.Enabled = true;

                tbTempo.Value = (int)current.Tempo;
                tbVolume.Value = current.Volume;
            }
            else
            {
                cmdPlay.Enabled = false;
                tbTempo.Enabled = false;
                tbVolume.Enabled = false;
            }

            lblTempo.Text = (tbTempo.Value > 0 ? "+" : "") + tbTempo.Value + "%";
            lblVolume.Text = tbVolume.Value + "";
        }

        private void Play(object sender, EventArgs e)
        {
            if (current != null)
            {
                current.Play(true);
            }
        }
    }
}
