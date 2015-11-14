namespace WorkTimer
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.cmoTimers = new System.Windows.Forms.ComboBox();
            this.cmdStop = new System.Windows.Forms.Button();
            this.cmdStart = new System.Windows.Forms.Button();
            this.cmdAddTimer = new System.Windows.Forms.Button();
            this.cmdDeleteTimer = new System.Windows.Forms.Button();
            this.lblTime = new System.Windows.Forms.Label();
            this.cmdOptions = new System.Windows.Forms.Button();
            this.cmdPause = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // cmoTimers
            // 
            this.cmoTimers.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmoTimers.FormattingEnabled = true;
            this.cmoTimers.Location = new System.Drawing.Point(13, 13);
            this.cmoTimers.Name = "cmoTimers";
            this.cmoTimers.Size = new System.Drawing.Size(169, 21);
            this.cmoTimers.TabIndex = 0;
            this.cmoTimers.SelectedIndexChanged += new System.EventHandler(this.cmoTimers_SelectedIndexChanged);
            // 
            // cmdStop
            // 
            this.cmdStop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdStop.Image = ((System.Drawing.Image)(resources.GetObject("cmdStop.Image")));
            this.cmdStop.Location = new System.Drawing.Point(162, 122);
            this.cmdStop.Name = "cmdStop";
            this.cmdStop.Size = new System.Drawing.Size(48, 49);
            this.cmdStop.TabIndex = 1;
            this.cmdStop.UseVisualStyleBackColor = false;
            // 
            // cmdStart
            // 
            this.cmdStart.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdStart.Image = ((System.Drawing.Image)(resources.GetObject("cmdStart.Image")));
            this.cmdStart.Location = new System.Drawing.Point(54, 122);
            this.cmdStart.Name = "cmdStart";
            this.cmdStart.Size = new System.Drawing.Size(48, 49);
            this.cmdStart.TabIndex = 2;
            this.cmdStart.UseVisualStyleBackColor = false;
            // 
            // cmdAddTimer
            // 
            this.cmdAddTimer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdAddTimer.Image = ((System.Drawing.Image)(resources.GetObject("cmdAddTimer.Image")));
            this.cmdAddTimer.Location = new System.Drawing.Point(188, 13);
            this.cmdAddTimer.Name = "cmdAddTimer";
            this.cmdAddTimer.Size = new System.Drawing.Size(22, 21);
            this.cmdAddTimer.TabIndex = 3;
            this.cmdAddTimer.UseVisualStyleBackColor = false;
            this.cmdAddTimer.Click += new System.EventHandler(this.AddTimer);
            // 
            // cmdDeleteTimer
            // 
            this.cmdDeleteTimer.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdDeleteTimer.Image = ((System.Drawing.Image)(resources.GetObject("cmdDeleteTimer.Image")));
            this.cmdDeleteTimer.Location = new System.Drawing.Point(216, 13);
            this.cmdDeleteTimer.Name = "cmdDeleteTimer";
            this.cmdDeleteTimer.Size = new System.Drawing.Size(22, 21);
            this.cmdDeleteTimer.TabIndex = 4;
            this.cmdDeleteTimer.UseVisualStyleBackColor = false;
            // 
            // lblTime
            // 
            this.lblTime.AutoSize = true;
            this.lblTime.Font = new System.Drawing.Font("Arial", 48F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTime.ForeColor = System.Drawing.Color.Red;
            this.lblTime.Location = new System.Drawing.Point(3, 47);
            this.lblTime.Name = "lblTime";
            this.lblTime.Size = new System.Drawing.Size(282, 72);
            this.lblTime.TabIndex = 5;
            this.lblTime.Text = "00:00:00";
            // 
            // cmdOptions
            // 
            this.cmdOptions.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdOptions.Image = ((System.Drawing.Image)(resources.GetObject("cmdOptions.Image")));
            this.cmdOptions.Location = new System.Drawing.Point(244, 13);
            this.cmdOptions.Name = "cmdOptions";
            this.cmdOptions.Size = new System.Drawing.Size(22, 21);
            this.cmdOptions.TabIndex = 8;
            this.cmdOptions.UseVisualStyleBackColor = false;
            // 
            // cmdPause
            // 
            this.cmdPause.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.cmdPause.Image = ((System.Drawing.Image)(resources.GetObject("cmdPause.Image")));
            this.cmdPause.Location = new System.Drawing.Point(108, 122);
            this.cmdPause.Name = "cmdPause";
            this.cmdPause.Size = new System.Drawing.Size(48, 49);
            this.cmdPause.TabIndex = 6;
            this.cmdPause.UseVisualStyleBackColor = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.ClientSize = new System.Drawing.Size(276, 182);
            this.Controls.Add(this.cmdOptions);
            this.Controls.Add(this.cmdPause);
            this.Controls.Add(this.lblTime);
            this.Controls.Add(this.cmdDeleteTimer);
            this.Controls.Add(this.cmdAddTimer);
            this.Controls.Add(this.cmdStart);
            this.Controls.Add(this.cmdStop);
            this.Controls.Add(this.cmoTimers);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "MainForm";
            this.Text = "Work Timer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Exit);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmoTimers;
        private System.Windows.Forms.Button cmdStop;
        private System.Windows.Forms.Button cmdStart;
        private System.Windows.Forms.Button cmdAddTimer;
        private System.Windows.Forms.Button cmdDeleteTimer;
        private System.Windows.Forms.Label lblTime;
        private System.Windows.Forms.Button cmdOptions;
        private System.Windows.Forms.Button cmdPause;
    }
}

