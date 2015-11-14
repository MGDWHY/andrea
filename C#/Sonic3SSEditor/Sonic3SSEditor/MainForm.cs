using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BlueSpheresForeverEditor
{
    public partial class MainForm : Form
    {
        private static readonly String[] DIRECTIONS = { "Left", "Right", "Up", "Down" };

        private static readonly float EDIT_BOARD_SCALING = 16.0f;
        private static readonly float EDIT_BOARD_OFFSET = -0.5f;

        public static readonly String DATA_FILE_FILTER = "Data files (*.dat)|*.dat";
        public static readonly String STAGE_FILE_FILTER_OPEN = "Blue Spheres Forever Stage (*.bss, *.dat)|*.bss;*.dat";
        public static readonly String STAGE_FILE_FILTER_SAVE = "Blue Spheres Forever Stage (*.bss)|*.bss";

        private BufferedGraphicsContext graphicsContext;
        private BufferedGraphics graphics;

        private GameData currentFileData;
        private String currentFileName;

        private TextureBrush backgroundImage;
        private TextureBrush[] toolImages;

        private int selectedTool;


        private static float WindowToView(int position)
        {
            return position / MainForm.EDIT_BOARD_SCALING + MainForm.EDIT_BOARD_OFFSET;
        }

        private static int ViewToWindow(int position)
        {
            return (int)((position - MainForm.EDIT_BOARD_OFFSET) * MainForm.EDIT_BOARD_SCALING);
        }

        public MainForm()
        {
            InitializeComponent();

            this.currentFileData = new GameData(32, 32);
            this.currentFileName = null;

            this.toolImages = new TextureBrush[7];

            for (int i = 0; i < this.toolImages.Length; i++)
            {
                this.toolImages[i] = new TextureBrush(Image.FromFile("images/" + i + ".png"));
            }

            this.backgroundImage = new TextureBrush(Image.FromFile("images/checkerboard.png"));

            this.selectedTool = 1;

            this.graphicsContext = BufferedGraphicsManager.Current;
            this.graphics = graphicsContext.Allocate(this.StageEditBoard.CreateGraphics(),
                new Rectangle(0, 0, 32 * (int)EDIT_BOARD_SCALING, 32 * (int)EDIT_BOARD_SCALING));

            for(int i = 0; i < MainForm.DIRECTIONS.Length; i++)
                this.StartDirection.Items.Add(DIRECTIONS[i]);
            

            this.LoadTextureFiles();

            this.FileNew(null, null);

        }


        #region EventHandlers


        private void OptionControls(object sender, EventArgs e)
        {
            lblCheckerColor1.Enabled = rdoChecker.Checked;
            lblCheckerColor2.Enabled = rdoChecker.Checked;
            TextureFileName.Enabled = !rdoChecker.Checked;
            BumpMapFileName.Enabled = chkBumpMapping.Checked;

        }  

        private void Exit(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void FileOpen(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = MainForm.STAGE_FILE_FILTER_OPEN;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.currentFileName = dialog.FileName;
                this.currentFileData = GameData.LoadData(this.currentFileName);
            }

            this.UpdateFormCaption();
            this.UpdateStagePropertiesPanel();
            this.StageEditBoard.Invalidate();
        }


        private void PaintStageEditBoard(object sender, PaintEventArgs e)
        {
            if (sender == this.StageEditBoard)
            {

                // Render background

                Graphics g = this.graphics.Graphics;


                Rectangle target = new Rectangle(
                    0, 0,
                    (int)MainForm.EDIT_BOARD_SCALING * (int)this.currentFileData.Width, 
                    (int)MainForm.EDIT_BOARD_SCALING * (int)this.currentFileData.Width
                );

                g.FillRectangle(this.backgroundImage, target);
                

                // Render objects

                for(int j = 0; j < this.currentFileData.Depth; j++)
                    for (int i = 0; i < this.currentFileData.Width; i++)
                    {
                        int imageIndex = (int)this.currentFileData.GetValueAt(i, j);
                        target = new Rectangle(
                            MainForm.ViewToWindow(i) - (int)MainForm.EDIT_BOARD_SCALING/2,
                            MainForm.ViewToWindow(j) - (int)MainForm.EDIT_BOARD_SCALING/2,
                            (int)MainForm.EDIT_BOARD_SCALING, (int)MainForm.EDIT_BOARD_SCALING);

                        g.FillRectangle(this.toolImages[imageIndex], target);

                        if (this.currentFileData.GetAvoidSearchAt(i, j) == GameData.AS_YES)
                            g.FillRectangle(this.toolImages[6], target);
                    }

                graphics.Render(e.Graphics);
            }
        }

        private void ChangeTool(object sender, EventArgs e)
        {
            if (sender == this.redSphereToolStripMenuItem)
                this.selectedTool = 1;
            else if (sender == this.blueSphereToolStripMenuItem)
                this.selectedTool = 2;
            else if (sender == this.starSphereToolStripMenuItem)
                this.selectedTool = 3;
            else if (sender == this.ringToolStripMenuItem)
                this.selectedTool = 4;
            else if (sender == this.yellowSphereToolStripMenuItem)
                this.selectedTool = 5;
            else if (sender == this.avoidSearchToolStripMenuItem)
                this.selectedTool = 6;

            this.UpdateMouseCursor();
        }

        private void EditStageBoardMouseEvents(object sender, MouseEventArgs e)
        {
            if (sender == this.StageEditBoard)
            {

                float viewX = MainForm.WindowToView(e.X);
                float viewZ = MainForm.WindowToView(e.Y);

                int x = (int)Math.Round(viewX);
                int z = (int)Math.Round(viewZ);

                if (x >= 0 && z >= 0 && x < this.currentFileData.Width && z < this.currentFileData.Depth)
                {
                    if (e.Button == MouseButtons.Left)
                    {
                        if (this.selectedTool == 6)
                        {
                            this.currentFileData.SetAvoidSearchAt(x, z, GameData.AS_YES);
                        }
                        else
                        {
                            this.currentFileData.SetValutAt(x, z, (uint)this.selectedTool);
                        }
                        this.StageEditBoard.Invalidate();
                    }
                    else if (e.Button == MouseButtons.Right)
                    {
                        this.currentFileData.SetAvoidSearchAt(x, z, GameData.AS_NO);
                        this.currentFileData.SetValutAt(x, z, GameData.C_NOTHING);
                        this.StageEditBoard.Invalidate();
                    }
                }

                this.CursorX.Text = "X: " + x;
                this.CursorZ.Text = "Z: " + z;
            }
        }

        private void FileSave(object sender, EventArgs e)
        {

            this.UpdateStageProperties();

            if (currentFileName == null || !currentFileName.EndsWith(".bss"))
                this.FileSaveAs(null, null);
            else
                GameData.StoreData(this.currentFileData, this.currentFileName);
        }

        private void FileSaveAs(object sender, EventArgs e)
        {

            SaveFileDialog dialog = new SaveFileDialog();

            dialog.Filter = MainForm.STAGE_FILE_FILTER_SAVE;

            this.UpdateStageProperties();

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.currentFileName = dialog.FileName;
                GameData.StoreData(this.currentFileData, this.currentFileName);
            }

            this.UpdateFormCaption();
        }



        private void FileNew(object sender, EventArgs e)
        {
            this.currentFileName = null;
            this.currentFileData = new GameData(32, 32);
            this.StageEditBoard.Invalidate();
            this.UpdateFormCaption();
            this.UpdateStagePropertiesPanel();
        }

        private void ShowAbout(object sender, EventArgs e)
        {
            new AboutForm().ShowDialog(this);
        }

        private void ShowGameStages(object sender, EventArgs e)
        {
            GameStagesForm gameStagesForm = new GameStagesForm();
            gameStagesForm.ShowDialog(this);
        }

        #endregion

        private void LoadTextureFiles()
        {
            if (Directory.Exists("textures"))
            {
                String[] textureFiles = Directory.GetFiles("textures");

                foreach (String file in textureFiles)
                {
                    if (file.EndsWith(".png"))
                    {
                        TextureFileName.Items.Add(Path.GetFileName(file));
                        BumpMapFileName.Items.Add(Path.GetFileName(file));
                    }
                }
            }
        }

        private void UpdateMouseCursor(){}



        private void UpdateFormCaption()
        {
            if (this.currentFileName == null)
                this.Text = "Blue Spheres Forever - Level Editor";
            else
                this.Text = "Blue Spheres Forever - Level Editor (" + this.currentFileName + ")";
        }

        private void UpdateStageProperties()
        {

            if (this.rdoChecker.Checked == true)
                this.currentFileData.FloorRenderingMode = GameData.RM_CHECKERBOARD;
            else if (this.rdoTexture.Checked == true)
                this.currentFileData.FloorRenderingMode = GameData.RM_TEXTURE;

            if (this.chkBumpMapping.Checked == true)
                this.currentFileData.BumpMappingEnabled = GameData.AS_YES;
            else
                this.currentFileData.BumpMappingEnabled = GameData.AS_NO;

            this.currentFileData.Texture = this.TextureFileName.Text;
            this.currentFileData.BumpMap = this.BumpMapFileName.Text;

            this.currentFileData.StartPoint.Data[0] = Int32.Parse(this.StartPositionX.Text);
            this.currentFileData.StartPoint.Data[1] = Int32.Parse(this.StartPositionZ.Text);

            if (this.StartDirection.SelectedIndex == 0) 
            { // left
                this.currentFileData.StartDirection.Data[0] = -1;
                this.currentFileData.StartDirection.Data[1] = 0;
            }
            else if(this.StartDirection.SelectedIndex == 1)
            { // right
                this.currentFileData.StartDirection.Data[0] = 1;
                this.currentFileData.StartDirection.Data[1] = 0;
            }
            else if (this.StartDirection.SelectedIndex == 2)
            { // up
                this.currentFileData.StartDirection.Data[0] = 0;
                this.currentFileData.StartDirection.Data[1] = -1;
            }
            else if (this.StartDirection.SelectedIndex == 3)
            { // down
                this.currentFileData.StartDirection.Data[0] = 0;
                this.currentFileData.StartDirection.Data[1] = 1;
            }

            this.currentFileData.MaxRings = UInt32.Parse(this.MaxRings.Text);

            this.currentFileData.EmeraldColor = MainForm.ColorToVecGeneric(this.EmeraldColor.BackColor);
            this.currentFileData.CheckerColor1 = MainForm.ColorToVecGeneric(this.lblCheckerColor1.BackColor);
            this.currentFileData.CheckerColor2 = MainForm.ColorToVecGeneric(this.lblCheckerColor2.BackColor);
            this.currentFileData.SkyBackcolor1 = MainForm.ColorToVecGeneric(this.lblSkyBackcolor1.BackColor);
            this.currentFileData.SkyBackcolor2 = MainForm.ColorToVecGeneric(this.lblSkyBackcolor2.BackColor);
            this.currentFileData.StarsColor1 = MainForm.ColorToVecGeneric(this.lblStarsColor1.BackColor);
            this.currentFileData.StarsColor2 = MainForm.ColorToVecGeneric(this.lblStarsColor2.BackColor);
        }

        private void UpdateStagePropertiesPanel()
        {

            if (this.currentFileData.FloorRenderingMode == GameData.RM_CHECKERBOARD)
            {
                this.rdoChecker.Checked = true;
                this.rdoTexture.Checked = false;
            }
            else if (this.currentFileData.FloorRenderingMode == GameData.RM_TEXTURE)
            {
                this.rdoChecker.Checked = false;
                this.rdoTexture.Checked = true;
            }

            if (this.currentFileData.BumpMappingEnabled == GameData.AS_YES)
                this.chkBumpMapping.Checked = true;
            else
                this.chkBumpMapping.Checked = false;

            this.TextureFileName.Text = this.currentFileData.Texture;
            this.BumpMapFileName.Text = this.currentFileData.BumpMap;

            this.StartPositionX.Text = this.currentFileData.StartPoint.Data[0] + "";
            this.StartPositionZ.Text = this.currentFileData.StartPoint.Data[1] + "";

            if (this.currentFileData.StartDirection.Data[0] == -1) // left
                this.StartDirection.SelectedIndex = 0;
            else if (this.currentFileData.StartDirection.Data[0] == 1) // right
                this.StartDirection.SelectedIndex = 1;
            else if (this.currentFileData.StartDirection.Data[1] == -1) // up
                this.StartDirection.SelectedIndex = 2;
            else if (this.currentFileData.StartDirection.Data[1] == 1) // down
                this.StartDirection.SelectedIndex = 3;

            this.MaxRings.Text = this.currentFileData.MaxRings + "";

            this.EmeraldColor.BackColor = MainForm.VecGenericToColor(this.currentFileData.EmeraldColor);

            this.lblCheckerColor1.BackColor = MainForm.VecGenericToColor(this.currentFileData.CheckerColor1);
            this.lblCheckerColor2.BackColor = MainForm.VecGenericToColor(this.currentFileData.CheckerColor2);

            this.lblSkyBackcolor1.BackColor = MainForm.VecGenericToColor(this.currentFileData.SkyBackcolor1);
            this.lblSkyBackcolor2.BackColor = MainForm.VecGenericToColor(this.currentFileData.SkyBackcolor2);

            this.lblStarsColor1.BackColor = MainForm.VecGenericToColor(this.currentFileData.StarsColor1);
            this.lblStarsColor2.BackColor = MainForm.VecGenericToColor(this.currentFileData.StarsColor2);

        }

        private static void ColorSelect(Label destination)
        {
            ColorDialog dialog = new ColorDialog();

            dialog.Color = destination.BackColor;

            if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                destination.BackColor = dialog.Color;
            }
        }

        private static Color VecGenericToColor(VecGeneric<float> v)
        {
            return Color.FromArgb((int)(v.Data[0] * 255), (int)(v.Data[1] * 255), (int)(v.Data[2] * 255));
        }

        private static VecGeneric<float> ColorToVecGeneric(Color color) {
            VecGeneric<float> v = new VecGeneric<float>(3);
            v.Data[0] = color.R / 255.0f;
            v.Data[1] = color.G / 255.0f;
            v.Data[2] = color.B / 255.0f;
            return v;
        }

        private void ChooseColor(object sender, EventArgs e)
        {
            MainForm.ColorSelect((Label)sender);
        } 
    }

}
