namespace SoundEngineTest
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
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            this.cmdOpen = new System.Windows.Forms.Button();
            this.lblFileName = new System.Windows.Forms.Label();
            this.cmdPlay = new System.Windows.Forms.Button();
            this.tbTempo = new System.Windows.Forms.TrackBar();
            this.tbVolume = new System.Windows.Forms.TrackBar();
            this.lblVolume = new System.Windows.Forms.Label();
            this.lblTempo = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.tbTempo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label1.Location = new System.Drawing.Point(12, 85);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(38, 14);
            label1.TabIndex = 5;
            label1.Text = "Tempo";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label2.Location = new System.Drawing.Point(12, 53);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(42, 14);
            label2.TabIndex = 6;
            label2.Text = "Volume";
            // 
            // cmdOpen
            // 
            this.cmdOpen.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdOpen.Location = new System.Drawing.Point(13, 13);
            this.cmdOpen.Name = "cmdOpen";
            this.cmdOpen.Size = new System.Drawing.Size(75, 23);
            this.cmdOpen.TabIndex = 0;
            this.cmdOpen.Text = "Open";
            this.cmdOpen.UseVisualStyleBackColor = true;
            this.cmdOpen.Click += new System.EventHandler(this.OpenFile);
            // 
            // lblFileName
            // 
            this.lblFileName.AutoSize = true;
            this.lblFileName.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblFileName.Location = new System.Drawing.Point(94, 18);
            this.lblFileName.Name = "lblFileName";
            this.lblFileName.Size = new System.Drawing.Size(81, 14);
            this.lblFileName.TabIndex = 1;
            this.lblFileName.Text = "No file selected";
            // 
            // cmdPlay
            // 
            this.cmdPlay.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdPlay.Location = new System.Drawing.Point(12, 119);
            this.cmdPlay.Name = "cmdPlay";
            this.cmdPlay.Size = new System.Drawing.Size(75, 23);
            this.cmdPlay.TabIndex = 2;
            this.cmdPlay.Text = "Play";
            this.cmdPlay.UseVisualStyleBackColor = true;
            this.cmdPlay.Click += new System.EventHandler(this.Play);
            // 
            // tbTempo
            // 
            this.tbTempo.Location = new System.Drawing.Point(70, 85);
            this.tbTempo.Maximum = 50;
            this.tbTempo.Minimum = -50;
            this.tbTempo.Name = "tbTempo";
            this.tbTempo.Size = new System.Drawing.Size(104, 45);
            this.tbTempo.TabIndex = 3;
            this.tbTempo.ValueChanged += new System.EventHandler(this.UpdateSound);
            // 
            // tbVolume
            // 
            this.tbVolume.Location = new System.Drawing.Point(70, 53);
            this.tbVolume.Maximum = 100;
            this.tbVolume.Name = "tbVolume";
            this.tbVolume.Size = new System.Drawing.Size(104, 45);
            this.tbVolume.TabIndex = 4;
            this.tbVolume.ValueChanged += new System.EventHandler(this.UpdateSound);
            // 
            // lblVolume
            // 
            this.lblVolume.AutoSize = true;
            this.lblVolume.Location = new System.Drawing.Point(180, 54);
            this.lblVolume.Name = "lblVolume";
            this.lblVolume.Size = new System.Drawing.Size(57, 13);
            this.lblVolume.TabIndex = 7;
            this.lblVolume.Text = "VolumeVal";
            // 
            // lblTempo
            // 
            this.lblTempo.AutoSize = true;
            this.lblTempo.Location = new System.Drawing.Point(180, 86);
            this.lblTempo.Name = "lblTempo";
            this.lblTempo.Size = new System.Drawing.Size(67, 13);
            this.lblTempo.TabIndex = 8;
            this.lblTempo.Text = "TempoValue";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(214, 147);
            this.Controls.Add(this.lblTempo);
            this.Controls.Add(this.lblVolume);
            this.Controls.Add(this.cmdPlay);
            this.Controls.Add(label2);
            this.Controls.Add(this.tbTempo);
            this.Controls.Add(label1);
            this.Controls.Add(this.tbVolume);
            this.Controls.Add(this.lblFileName);
            this.Controls.Add(this.cmdOpen);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "SoundTest";
            ((System.ComponentModel.ISupportInitialize)(this.tbTempo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbVolume)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdOpen;
        private System.Windows.Forms.Label lblFileName;
        private System.Windows.Forms.Button cmdPlay;
        private System.Windows.Forms.TrackBar tbTempo;
        private System.Windows.Forms.TrackBar tbVolume;
        private System.Windows.Forms.Label lblVolume;
        private System.Windows.Forms.Label lblTempo;
    }
}

