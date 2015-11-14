using System;
using System.Windows.Forms;
using System.IO;

namespace BlueSpheresForeverLauncher
{
    public partial class LauncherForm : Form
    {

        private GameConfig gameConfig;

        public LauncherForm()
        {
            InitializeComponent();

            cmoDisplayMode.Items.AddRange(DisplayModesEnumerator.GetAvailableDisplayModes().ToArray());

            gameConfig = GameConfig.Load();

            UpdateControls();

        }

        private void UpdateControls()
        {
            cmoQuality.SelectedIndex = gameConfig.Quality;
            cmoDisplayMode.SelectedItem = gameConfig.DisplayMode;
            cmoAnisotropicFilter.SelectedItem = "" + gameConfig.AnisotropicFilter;
            cmoWindowed.Checked = gameConfig.Windowed == 1 ? true : false;
        }

        private void LaunchGame(object sender, EventArgs e)
        {
            gameConfig.DisplayMode = (DisplayMode)cmoDisplayMode.SelectedItem;
            gameConfig.Quality = cmoQuality.SelectedIndex;
            gameConfig.AnisotropicFilter = Int32.Parse(cmoAnisotropicFilter.SelectedItem.ToString());
            gameConfig.Windowed = cmoWindowed.Checked ? 1 : 0;
            GameConfig.Save(gameConfig);
            System.Diagnostics.Process.Start(Path.GetDirectoryName(Application.ExecutablePath) + "/bsf.exe");
            Application.Exit();
        }

        private void Exit(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
