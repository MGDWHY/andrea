using System.Runtime.InteropServices;
using System.IO;
using System.Windows.Forms;
using System;

namespace BlueSpheresForeverLauncher
{

    [StructLayout(LayoutKind.Sequential)]
    struct GameConfig
    {
        public const string CONFIG_FILE = "config.cfg";
        public const string FIRST_TIME_FILE = "first.time";

        public DisplayMode DisplayMode;
        public int Windowed;
        public int Quality;
        public int AnisotropicFilter;
        public int Stereo3D;
        public int ShadowsEnabled;
        public int ShadowQuality;
        public int MusicVolume;
        public int FXVolume;

        private static GameConfig GetDefaultGameConfig()
        {
            GameConfig gameConfig = new GameConfig();
            gameConfig.DisplayMode = DisplayModesEnumerator.GetAvailableDisplayModes()[0];
            gameConfig.Windowed = 1;
            gameConfig.AnisotropicFilter = 4;
            gameConfig.Quality = 2; // High quality
            gameConfig.Stereo3D = 0; // Disabled
            gameConfig.ShadowsEnabled = 1; // Enabled
            gameConfig.ShadowQuality = 2048; // High
            gameConfig.MusicVolume = 100;
            gameConfig.FXVolume = 100;
            return gameConfig;
        }

        public static void Save(GameConfig config)
        {
            string configFile = Path.GetDirectoryName(Application.ExecutablePath) + "/" + CONFIG_FILE;
            BinaryWriter writer = new BinaryWriter(new FileStream(configFile, FileMode.Create));

            writer.Write(config.DisplayMode.Width);
            writer.Write(config.DisplayMode.Height);
            writer.Write(config.DisplayMode.Frequency);
            writer.Write(config.DisplayMode.BitsPerPixel);
            writer.Write(config.Windowed);
            writer.Write(config.Quality);
            writer.Write(config.AnisotropicFilter);
            writer.Write(config.Stereo3D);
            writer.Write(config.ShadowsEnabled);
            writer.Write(config.ShadowQuality);
            writer.Write(config.MusicVolume);
            writer.Write(config.FXVolume);

            writer.Close();
        }

        public static GameConfig Load()
        {
            GameConfig result = GetDefaultGameConfig();

            string firstTimeFile = Path.GetDirectoryName(Application.ExecutablePath) + "/" + FIRST_TIME_FILE;
            string configFile = Path.GetDirectoryName(Application.ExecutablePath) + "/" + CONFIG_FILE;

            try
            {
                BinaryReader reader = new BinaryReader(new FileStream(firstTimeFile, FileMode.Open));

                byte firstTime = reader.ReadByte();

                reader.Close();

                if (firstTime == 0)
                {

                    BinaryWriter writer = new BinaryWriter(new FileStream(firstTimeFile, FileMode.Create));

                    writer.Write((byte)1);

                    writer.Close();

                    return result;
                }
            }
            catch (FileNotFoundException){}

            try
            {
                BinaryReader reader = new BinaryReader(new FileStream(configFile, FileMode.Open));
                result.DisplayMode = new DisplayMode(
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32(),
                    reader.ReadInt32()
                );

                result.Windowed = reader.ReadInt32();
                result.Quality = reader.ReadInt32();
                result.AnisotropicFilter = reader.ReadInt32();
                result.Stereo3D = reader.ReadInt32();
                result.ShadowsEnabled = reader.ReadInt32();
                result.ShadowQuality = reader.ReadInt32();
                result.MusicVolume = reader.ReadInt32();
                result.FXVolume = reader.ReadInt32();

                reader.Close();

            }
            catch (FileNotFoundException){}

            return result;
        }
    }
}
