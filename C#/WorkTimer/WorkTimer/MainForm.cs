using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WorkTimer
{
    public partial class MainForm : Form
    {

        private List<WorkTimer> timers;

        private WorkTimer CurrentTimer
        {
            get { return (WorkTimer)cmoTimers.SelectedItem; }
        }

        private void InitializeTimers()
        {
            if (File.Exists(Program.DATA_FILE))
                timers = Program.LoadData(Program.DATA_FILE);
            else
                timers = new List<WorkTimer>();

            cmoTimers.Items.AddRange(timers.ToArray());
        }

        public MainForm()
        {
            InitializeComponent();
            InitializeTimers();
        }

        private void Exit(object sender, FormClosedEventArgs e)
        {
            Program.SaveData(timers, Program.DATA_FILE);
        }

        private void AddTimer(object sender, EventArgs e)
        {
            InputBoxResult result = InputBox.Prompt("Enter the name for the timer");

            if (result.Result == DialogResult.OK && result.Value.Length > 0)
            {
                PauseAll();
                timers.Add(new WorkTimer(result.Value));
            }
        }

        private void PauseAll()
        {
            foreach (WorkTimer t in timers)
            {
                t.Active = false;
                t.Tick = null;
            }
        }

        private void StopCurrent()
        {
            if (CurrentTimer != null)
            {
                CurrentTimer.Active = false;
                CurrentTimer.WorkTime = 0;
            }
        }

        private void PauseCurrent()
        {
            if (CurrentTimer != null)
            {
                CurrentTimer.Active = false;
            }
        }

        private void RunCurrent()
        {
            if (CurrentTimer != null)
            {
                CurrentTimer.Active = true;
            }
        }

        private void UpdateTime()
        {
            if (CurrentTimer != null)
            {
                int h = CurrentTimer.WorkTime / 3600;
                int m = CurrentTimer.WorkTime / 60 - h * 60;
                int s = CurrentTimer.WorkTime - m * 60 - h * 3600;
                lblTime.Text = h + ":" + m + ":" + s;
            }
            else
                lblTime.Text = "00:00:00";
        }

        private void cmoTimers_SelectedIndexChanged(object sender, EventArgs e)
        {
            PauseAll();
            if (CurrentTimer != null)
            {
                CurrentTimer.Tick = UpdateTime;
            }
        }
    }
}
