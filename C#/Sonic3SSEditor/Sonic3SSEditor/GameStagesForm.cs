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
    public partial class GameStagesForm : Form
    {

        private GameStages gameStages;

        public GameStagesForm()
        {
            InitializeComponent();

            this.gameStages = GameStages.Load();
            this.UpdateList();
        }

        private void UpdateList()
        {
            this.lstGameStages.Items.Clear();
            this.lstGameStages.Items.AddRange(this.gameStages.ToArray());
        }

        private void UpdateList(int selectedIndex)
        {
            this.lstGameStages.Items.Clear();
            this.lstGameStages.Items.AddRange(this.gameStages.ToArray());
            this.lstGameStages.SelectedIndex = selectedIndex;
        }

        private void SwapItems(int i1, int i2)
        {
            if (i1 < gameStages.Count && i2 < gameStages.Count
                && i1 >= 0 && i2 >= 0)
            {
                string el1 = this.gameStages[i1];
                this.gameStages[i1] = this.gameStages[i2];
                this.gameStages[i2] = el1;
                this.UpdateList(i2);    
            }
    
        }

        #region EventHandlers

        private void FormClose(object sender, FormClosingEventArgs e)
        {
            GameStages.Store(this.gameStages);
        }

        private void AddStage(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = MainForm.STAGE_FILE_FILTER_SAVE;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.gameStages.Add(dialog.SafeFileName);
                this.UpdateList();
            }
        }

        private void DeleteStage(object sender, EventArgs e)
        {
            if (this.lstGameStages.SelectedIndex >= 0)
            {
                this.gameStages.RemoveAt(this.lstGameStages.SelectedIndex);
                this.UpdateList();
            }
        }

        private void MoveUpStage(object sender, EventArgs e)
        {
            this.SwapItems(this.lstGameStages.SelectedIndex, this.lstGameStages.SelectedIndex - 1);
        }

        private void MoveDownStage(object sender, EventArgs e)
        {
            this.SwapItems(this.lstGameStages.SelectedIndex, this.lstGameStages.SelectedIndex + 1);
        }

        #endregion

    }
}
