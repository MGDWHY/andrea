using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Collections.Generic;
using System;

namespace BlueSpheresForeverLauncher
{
    public class DisplayModesEnumerator
    {
        public const int MIN_FREQUENCY = 60;
        public const int BITS_PER_PIXEL = 32;

        public static List<DisplayMode> GetAvailableDisplayModes()
        {
            List<DisplayMode> result = new List<DisplayMode>();
          
            for(int i = 0;; i++) {
                DEVMODE devMode = new DEVMODE();
                bool retval = EnumDisplaySettings(null, i, ref devMode);
                if (retval)
                {
                 
                    DisplayMode displayMode = new DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight,
                        devMode.dmDisplayFrequency, devMode.dmBitsPerPel);

                    if (!result.Contains(displayMode) && displayMode.Frequency >= MIN_FREQUENCY && displayMode.BitsPerPixel == BITS_PER_PIXEL)
                        result.Add(displayMode);

                }
                else
                    break;
            }
           

            return result;
        }



        [DllImport("user32.dll")]
        private static extern bool EnumDisplaySettings(
              string deviceName, int modeNum, ref DEVMODE devMode);

        [StructLayout(LayoutKind.Sequential)]
        private struct DEVMODE
        {

            private const int CCHDEVICENAME = 0x20;
            private const int CCHFORMNAME = 0x20;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmDeviceName;
            public short dmSpecVersion;
            public short dmDriverVersion;
            public short dmSize;
            public short dmDriverExtra;
            public int dmFields;
            public int dmPositionX;
            public int dmPositionY;
            public ScreenOrientation dmDisplayOrientation;
            public int dmDisplayFixedOutput;
            public short dmColor;
            public short dmDuplex;
            public short dmYResolution;
            public short dmTTOption;
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 0x20)]
            public string dmFormName;
            public short dmLogPixels;
            public int dmBitsPerPel;
            public int dmPelsWidth;
            public int dmPelsHeight;
            public int dmDisplayFlags;
            public int dmDisplayFrequency;
            public int dmICMMethod;
            public int dmICMIntent;
            public int dmMediaType;
            public int dmDitherType;
            public int dmReserved1;
            public int dmReserved2;
            public int dmPanningWidth;
            public int dmPanningHeight;
        }
    }

    public class DisplayMode : IEquatable<DisplayMode>
    {
        private int width, height;

        private int frequency;
        private int bpp;

        public DisplayMode(int width, int height, int frequency, int bpp)
        {
            this.width = width;
            this.height = height;
            this.frequency = frequency;
            this.bpp = bpp;
        }

        public int Width
        {
            get { return width; }
        }

        public int Height
        {
            get { return height; }
        }

        public int Frequency {
            get { return frequency; }
        }

        public int BitsPerPixel {
            get { return bpp; }
        }

        public bool Equals(DisplayMode other) {
            return this.Width == other.Width && this.Height == other.Height
                && this.Frequency == other.Frequency && this.BitsPerPixel == other.BitsPerPixel;
        }

        public override bool Equals(object obj)
        {
            DisplayMode other = (DisplayMode)obj;
            return this.Equals(other);
        }

        public override string ToString()
        {
            return width + "x" + height + " (" + frequency + "Hz)"; 
        }

    }
}
