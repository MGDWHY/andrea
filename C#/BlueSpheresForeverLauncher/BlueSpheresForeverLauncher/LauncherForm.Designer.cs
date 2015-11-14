namespace BlueSpheresForeverLauncher
{
    partial class LauncherForm
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
            System.Windows.Forms.PictureBox pictureBox1;
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label3;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LauncherForm));
            this.cmoDisplayMode = new System.Windows.Forms.ComboBox();
            this.cmoQuality = new System.Windows.Forms.ComboBox();
            this.cmdLaunchGame = new System.Windows.Forms.Button();
            this.cmdExit = new System.Windows.Forms.Button();
            this.cmoAnisotropicFilter = new System.Windows.Forms.ComboBox();
            this.cmoWindowed = new System.Windows.Forms.CheckBox();
            pictureBox1 = new System.Windows.Forms.PictureBox();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            pictureBox1.BackColor = System.Drawing.Color.Black;
            pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            pictureBox1.Image = global::BlueSpheresForeverLauncher.Properties.Resources.logo;
            pictureBox1.Location = new System.Drawing.Point(12, 12);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new System.Drawing.Size(305, 130);
            pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            pictureBox1.TabIndex = 0;
            pictureBox1.TabStop = false;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label1.Location = new System.Drawing.Point(9, 156);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(71, 14);
            label1.TabIndex = 1;
            label1.Text = "Display Mode";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label2.Location = new System.Drawing.Point(171, 208);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(40, 14);
            label2.TabIndex = 3;
            label2.Text = "Quality";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label3.Location = new System.Drawing.Point(12, 206);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(88, 14);
            label3.TabIndex = 7;
            label3.Text = "Anisotropic Filter";
            // 
            // cmoDisplayMode
            // 
            this.cmoDisplayMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmoDisplayMode.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmoDisplayMode.FormattingEnabled = true;
            this.cmoDisplayMode.Location = new System.Drawing.Point(12, 172);
            this.cmoDisplayMode.Name = "cmoDisplayMode";
            this.cmoDisplayMode.Size = new System.Drawing.Size(143, 22);
            this.cmoDisplayMode.TabIndex = 2;
            // 
            // cmoQuality
            // 
            this.cmoQuality.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmoQuality.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmoQuality.FormattingEnabled = true;
            this.cmoQuality.Items.AddRange(new object[] {
            "Low",
            "Medium",
            "High"});
            this.cmoQuality.Location = new System.Drawing.Point(174, 223);
            this.cmoQuality.Name = "cmoQuality";
            this.cmoQuality.Size = new System.Drawing.Size(143, 22);
            this.cmoQuality.TabIndex = 4;
            // 
            // cmdLaunchGame
            // 
            this.cmdLaunchGame.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdLaunchGame.Location = new System.Drawing.Point(12, 268);
            this.cmdLaunchGame.Name = "cmdLaunchGame";
            this.cmdLaunchGame.Size = new System.Drawing.Size(89, 23);
            this.cmdLaunchGame.TabIndex = 5;
            this.cmdLaunchGame.Text = "Launch Game";
            this.cmdLaunchGame.UseVisualStyleBackColor = true;
            this.cmdLaunchGame.Click += new System.EventHandler(this.LaunchGame);
            // 
            // cmdExit
            // 
            this.cmdExit.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmdExit.Location = new System.Drawing.Point(107, 268);
            this.cmdExit.Name = "cmdExit";
            this.cmdExit.Size = new System.Drawing.Size(89, 23);
            this.cmdExit.TabIndex = 6;
            this.cmdExit.Text = "Exit";
            this.cmdExit.UseVisualStyleBackColor = true;
            this.cmdExit.Click += new System.EventHandler(this.Exit);
            // 
            // cmoAnisotropicFilter
            // 
            this.cmoAnisotropicFilter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmoAnisotropicFilter.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmoAnisotropicFilter.FormattingEnabled = true;
            this.cmoAnisotropicFilter.Items.AddRange(new object[] {
            "1",
            "2",
            "4",
            "8",
            "16"});
            this.cmoAnisotropicFilter.Location = new System.Drawing.Point(12, 223);
            this.cmoAnisotropicFilter.Name = "cmoAnisotropicFilter";
            this.cmoAnisotropicFilter.Size = new System.Drawing.Size(143, 22);
            this.cmoAnisotropicFilter.TabIndex = 8;
            // 
            // cmoWindowed
            // 
            this.cmoWindowed.AutoSize = true;
            this.cmoWindowed.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cmoWindowed.Location = new System.Drawing.Point(174, 174);
            this.cmoWindowed.Name = "cmoWindowed";
            this.cmoWindowed.Size = new System.Drawing.Size(78, 18);
            this.cmoWindowed.TabIndex = 9;
            this.cmoWindowed.Text = "Windowed";
            this.cmoWindowed.UseVisualStyleBackColor = true;
            // 
            // LauncherForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(333, 303);
            this.Controls.Add(this.cmoWindowed);
            this.Controls.Add(this.cmoAnisotropicFilter);
            this.Controls.Add(label3);
            this.Controls.Add(this.cmdExit);
            this.Controls.Add(this.cmdLaunchGame);
            this.Controls.Add(this.cmoQuality);
            this.Controls.Add(label2);
            this.Controls.Add(this.cmoDisplayMode);
            this.Controls.Add(label1);
            this.Controls.Add(pictureBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "LauncherForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Blue Spheres Forever Launcher";
            ((System.ComponentModel.ISupportInitialize)(pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmoDisplayMode;
        private System.Windows.Forms.ComboBox cmoQuality;
        private System.Windows.Forms.Button cmdLaunchGame;
        private System.Windows.Forms.Button cmdExit;
        private System.Windows.Forms.ComboBox cmoAnisotropicFilter;
        private System.Windows.Forms.CheckBox cmoWindowed;

    }
}

