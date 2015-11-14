namespace Sonic3SSSectionDecoder
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
            System.Windows.Forms.GroupBox groupBox1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.Label label1;
            System.Windows.Forms.MenuStrip menuStrip1;
            System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
            System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
            System.Windows.Forms.StatusStrip statusStrip1;
            this.txtMaxRings = new System.Windows.Forms.TextBox();
            this.cmoSelectedSection = new System.Windows.Forms.ComboBox();
            this.mnuFile = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSave = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuExportData = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuExit = new System.Windows.Forms.ToolStripMenuItem();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.prbStatus = new System.Windows.Forms.ToolStripProgressBar();
            this.picSectionImage = new System.Windows.Forms.PictureBox();
            groupBox1 = new System.Windows.Forms.GroupBox();
            label2 = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            menuStrip1 = new System.Windows.Forms.MenuStrip();
            toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            statusStrip1 = new System.Windows.Forms.StatusStrip();
            groupBox1.SuspendLayout();
            menuStrip1.SuspendLayout();
            statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picSectionImage)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(this.txtMaxRings);
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(this.cmoSelectedSection);
            groupBox1.Controls.Add(label1);
            groupBox1.Dock = System.Windows.Forms.DockStyle.Right;
            groupBox1.Font = new System.Drawing.Font("Arial", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            groupBox1.Location = new System.Drawing.Point(469, 24);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(200, 474);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Sections";
            // 
            // txtMaxRings
            // 
            this.txtMaxRings.Location = new System.Drawing.Point(94, 62);
            this.txtMaxRings.Name = "txtMaxRings";
            this.txtMaxRings.Size = new System.Drawing.Size(44, 20);
            this.txtMaxRings.TabIndex = 4;
            this.txtMaxRings.Leave += new System.EventHandler(this.txtMaxRings_Leave);
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(6, 65);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(57, 14);
            label2.TabIndex = 3;
            label2.Text = "Max Rings";
            // 
            // cmoSelectedSection
            // 
            this.cmoSelectedSection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmoSelectedSection.FormattingEnabled = true;
            this.cmoSelectedSection.Location = new System.Drawing.Point(94, 26);
            this.cmoSelectedSection.Name = "cmoSelectedSection";
            this.cmoSelectedSection.Size = new System.Drawing.Size(90, 22);
            this.cmoSelectedSection.TabIndex = 1;
            this.cmoSelectedSection.SelectedIndexChanged += new System.EventHandler(this.cmoSelectedSection_SelectedIndexChanged);
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(6, 29);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(82, 14);
            label1.TabIndex = 0;
            label1.Text = "Current Section";
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuFile});
            menuStrip1.Location = new System.Drawing.Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new System.Drawing.Size(669, 24);
            menuStrip1.TabIndex = 2;
            menuStrip1.Text = "menuStrip1";
            // 
            // mnuFile
            // 
            this.mnuFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuOpen,
            this.mnuSave,
            toolStripMenuItem1,
            this.mnuExportData,
            toolStripMenuItem3,
            this.mnuExit});
            this.mnuFile.Name = "mnuFile";
            this.mnuFile.Size = new System.Drawing.Size(37, 20);
            this.mnuFile.Text = "&File";
            // 
            // mnuOpen
            // 
            this.mnuOpen.Name = "mnuOpen";
            this.mnuOpen.Size = new System.Drawing.Size(142, 22);
            this.mnuOpen.Text = "&Open...";
            this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
            // 
            // mnuSave
            // 
            this.mnuSave.Name = "mnuSave";
            this.mnuSave.Size = new System.Drawing.Size(142, 22);
            this.mnuSave.Text = "&Save...";
            this.mnuSave.Click += new System.EventHandler(this.mnuSave_Click);
            // 
            // toolStripMenuItem1
            // 
            toolStripMenuItem1.Name = "toolStripMenuItem1";
            toolStripMenuItem1.Size = new System.Drawing.Size(139, 6);
            // 
            // mnuExportData
            // 
            this.mnuExportData.Name = "mnuExportData";
            this.mnuExportData.Size = new System.Drawing.Size(142, 22);
            this.mnuExportData.Text = "Export data...";
            this.mnuExportData.Click += new System.EventHandler(this.mnuExportData_Click);
            // 
            // toolStripMenuItem3
            // 
            toolStripMenuItem3.Name = "toolStripMenuItem3";
            toolStripMenuItem3.Size = new System.Drawing.Size(139, 6);
            // 
            // mnuExit
            // 
            this.mnuExit.Name = "mnuExit";
            this.mnuExit.Size = new System.Drawing.Size(142, 22);
            this.mnuExit.Text = "&Exit";
            this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
            // 
            // statusStrip1
            // 
            statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus,
            this.prbStatus});
            statusStrip1.Location = new System.Drawing.Point(0, 476);
            statusStrip1.Name = "statusStrip1";
            statusStrip1.Size = new System.Drawing.Size(469, 22);
            statusStrip1.TabIndex = 3;
            statusStrip1.Text = "statusStrip1";
            // 
            // lblStatus
            // 
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(52, 17);
            this.lblStatus.Text = "lblStatus";
            // 
            // prbStatus
            // 
            this.prbStatus.Name = "prbStatus";
            this.prbStatus.Size = new System.Drawing.Size(100, 16);
            // 
            // picSectionImage
            // 
            this.picSectionImage.BackColor = System.Drawing.Color.Black;
            this.picSectionImage.Dock = System.Windows.Forms.DockStyle.Fill;
            this.picSectionImage.Location = new System.Drawing.Point(0, 24);
            this.picSectionImage.Name = "picSectionImage";
            this.picSectionImage.Size = new System.Drawing.Size(469, 474);
            this.picSectionImage.TabIndex = 1;
            this.picSectionImage.TabStop = false;
            this.picSectionImage.Paint += new System.Windows.Forms.PaintEventHandler(this.picSectionImage_Paint);
            this.picSectionImage.MouseDown += new System.Windows.Forms.MouseEventHandler(this.picSectionImage_MouseDown);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(669, 498);
            this.Controls.Add(statusStrip1);
            this.Controls.Add(this.picSectionImage);
            this.Controls.Add(groupBox1);
            this.Controls.Add(menuStrip1);
            this.Name = "MainForm";
            this.Text = "Sonic 3 Special Stage - Section Decoder";
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            statusStrip1.ResumeLayout(false);
            statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picSectionImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cmoSelectedSection;
        private System.Windows.Forms.PictureBox picSectionImage;
        private System.Windows.Forms.TextBox txtMaxRings;
        private System.Windows.Forms.ToolStripMenuItem mnuFile;
        private System.Windows.Forms.ToolStripMenuItem mnuOpen;
        private System.Windows.Forms.ToolStripMenuItem mnuSave;
        private System.Windows.Forms.ToolStripMenuItem mnuExit;
        private System.Windows.Forms.ToolStripMenuItem mnuExportData;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.Windows.Forms.ToolStripProgressBar prbStatus;
    }
}

