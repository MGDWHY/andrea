using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BlueSpheresForeverEditor
{
    class GameData
    {

        public static readonly uint VERSION_100 = 100;
        public static readonly uint VERSION_200 = 200;

        public static readonly uint C_NOTHING = 0;
        public static readonly uint C_REDSPHERE = 1;
        public static readonly uint C_BLUESPHERE = 2;
        public static readonly uint C_STARSPHERE = 3;
        public static readonly uint C_RING = 4;
        public static readonly uint C_YELLOWSPHERE = 5;

        public static readonly byte RM_CHECKERBOARD = 0;
        public static readonly byte RM_TEXTURE = 1;

        public static readonly byte AS_NO = 0;
        public static readonly byte AS_YES = 1;

        private uint version;

        private uint width, depth;

        private byte floorRenderingMode;

        private byte bumpMappingEnabled;

        private String texture, bumpMap;

        private VecGeneric<int> startPoint, startDirection;

        private VecGeneric<float> emeraldColor;

        private VecGeneric<float> checkerColor1, checkerColor2;

        private VecGeneric<float> skyBackcolor1, skyBackcolor2;

        private VecGeneric<float> starsColor1, starsColor2;

        private uint maxRings;

        private uint[] data;
        private byte[] avoidSearch;

        public uint Version
        {
            get { return version; }
            set { version = value; }
        }

        public uint Depth
        {
            get { return depth; }
        }

        public uint Width
        {
            get { return width; }
        }

        public byte FloorRenderingMode
        {
            get { return floorRenderingMode; }
            set { floorRenderingMode = value; }
        }

        public byte BumpMappingEnabled
        {
            get { return bumpMappingEnabled; }
            set { bumpMappingEnabled = value; }
        }

        public String BumpMap
        {
            get { return bumpMap; }
            set { bumpMap = value; }
        }

        public String Texture
        {
            get { return texture; }
            set { texture = value; }
        }

        public VecGeneric<int> StartDirection
        {
            get { return startDirection; }
            set { startDirection = value; }
        }

        public VecGeneric<int> StartPoint
        {
            get { return startPoint; }
            set { startPoint = value; }
        }

        public VecGeneric<float> EmeraldColor
        {
            get { return emeraldColor; }
            set { emeraldColor = value; }
        }

        public VecGeneric<float> CheckerColor1
        {
            get { return checkerColor1; }
            set { checkerColor1 = value; }
        }

        public VecGeneric<float> CheckerColor2
        {
            get { return checkerColor2; }
            set { checkerColor2 = value; }
        }

        public VecGeneric<float> SkyBackcolor1
        {
            get { return skyBackcolor1; }
            set { skyBackcolor1 = value; }
        }

        public VecGeneric<float> SkyBackcolor2
        {
            get { return skyBackcolor2; }
            set { skyBackcolor2 = value; }
        }

        public VecGeneric<float> StarsColor1
        {
            get { return starsColor1; }
            set { starsColor1 = value; }
        }

        public VecGeneric<float> StarsColor2
        {
            get { return starsColor2; }
            set { starsColor2 = value; }
        }

        public uint MaxRings
        {
            get { return maxRings; }
            set { maxRings = value; }
        }

        public uint[] Data
        {
            get { return data; }
            set { data = value; }
        }

        public byte[] AvoidSearch
        {
            get { return avoidSearch; }
            set { avoidSearch = value; }
        }

        public GameData(uint width, uint depth)
        {
            this.version = VERSION_200;

            this.width = width;
            this.depth = depth;
            this.data = new uint[width * depth];
            this.avoidSearch = new byte[width * depth];

            this.bumpMappingEnabled = AS_NO;
            this.floorRenderingMode = RM_CHECKERBOARD;

            this.texture = "";
            this.bumpMap = "";

            this.startPoint = new VecGeneric<int>(2);
            this.startDirection = new VecGeneric<int>(2);
            this.emeraldColor = new VecGeneric<float>(3);
            this.checkerColor1 = new VecGeneric<float>(3);
            this.checkerColor2 = new VecGeneric<float>(3);
            this.skyBackcolor1 = new VecGeneric<float>(3);
            this.skyBackcolor2 = new VecGeneric<float>(3);
            this.starsColor1 = new VecGeneric<float>(3);
            this.starsColor2 = new VecGeneric<float>(3);

            // Right
            this.startDirection.Data[0] = -1;
            this.startDirection.Data[1] = 0;

            for (int i = 0; i < width * depth; i++)
            {
                this.data[i] = GameData.C_NOTHING;
                this.avoidSearch[i] = GameData.AS_NO;
            }
        }

        public byte GetAvoidSearchAt(int x, int z)
        {
            return this.avoidSearch[z * this.width + x];
        }

        public void SetAvoidSearchAt(int x, int z, byte value)
        {
            this.avoidSearch[z * this.width + x] = value;
        }

        public uint GetValueAt(int x, int z)
        {
            return this.data[z * this.width + x];
        }

        public void SetValutAt(int x, int z, uint value)
        {
            this.data[z * this.width + x] = value;
        }

        public static GameData LoadData(String fileName)
        {
            FileStream fs = new FileStream(fileName, FileMode.Open);
            BinaryReader reader = new BinaryReader(fs);

            uint version = reader.ReadUInt32();

            GameData data;

            if (version == VERSION_100)
            {

                data = new GameData(reader.ReadUInt32(), reader.ReadUInt32());

                MessageBox.Show("This file is encoded in version #100. It will be converted to version #200", 
                    "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
                
                uint textureLenght = reader.ReadUInt32();
                uint bumpMapLenght = reader.ReadUInt32();

                data.FloorRenderingMode = RM_TEXTURE;
                data.BumpMappingEnabled = AS_YES;

                data.Texture = new String(reader.ReadChars((int)textureLenght));
                data.BumpMap = new String(reader.ReadChars((int)bumpMapLenght));

                data.StartPoint.Data[0] = reader.ReadInt32();
                data.StartPoint.Data[1] = reader.ReadInt32();

                data.StartDirection.Data[0] = reader.ReadInt32();
                data.StartDirection.Data[1] = reader.ReadInt32();

                data.EmeraldColor.Data[0] = reader.ReadSingle();
                data.EmeraldColor.Data[1] = reader.ReadSingle();
                data.EmeraldColor.Data[2] = reader.ReadSingle();

                data.MaxRings = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.Data[j * data.Width + i] = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.AvoidSearch[j * data.Width + i] = reader.ReadByte();

            }
            else if (version == VERSION_200)
            {
                
                data = new GameData(reader.ReadUInt32(), reader.ReadUInt32());

                data.FloorRenderingMode = reader.ReadByte();
                data.BumpMappingEnabled = reader.ReadByte();

                uint textureLenght = reader.ReadUInt32();
                uint bumpMapLenght = reader.ReadUInt32();

                data.Texture = new String(reader.ReadChars((int)textureLenght));
                data.BumpMap = new String(reader.ReadChars((int)bumpMapLenght));

                data.StartPoint.Data[0] = reader.ReadInt32();
                data.StartPoint.Data[1] = reader.ReadInt32();

                data.StartDirection.Data[0] = reader.ReadInt32();
                data.StartDirection.Data[1] = reader.ReadInt32();

                data.EmeraldColor.Data[0] = reader.ReadSingle();
                data.EmeraldColor.Data[1] = reader.ReadSingle();
                data.EmeraldColor.Data[2] = reader.ReadSingle();

                data.CheckerColor1.Data[0] = reader.ReadSingle();
                data.CheckerColor1.Data[1] = reader.ReadSingle();
                data.CheckerColor1.Data[2] = reader.ReadSingle();

                data.CheckerColor2.Data[0] = reader.ReadSingle();
                data.CheckerColor2.Data[1] = reader.ReadSingle();
                data.CheckerColor2.Data[2] = reader.ReadSingle();

                data.SkyBackcolor1.Data[0] = reader.ReadSingle();
                data.SkyBackcolor1.Data[1] = reader.ReadSingle();
                data.SkyBackcolor1.Data[2] = reader.ReadSingle();

                data.SkyBackcolor2.Data[0] = reader.ReadSingle();
                data.SkyBackcolor2.Data[1] = reader.ReadSingle();
                data.SkyBackcolor2.Data[2] = reader.ReadSingle();

                data.StarsColor1.Data[0] = reader.ReadSingle();
                data.StarsColor1.Data[1] = reader.ReadSingle();
                data.StarsColor1.Data[2] = reader.ReadSingle();

                data.StarsColor2.Data[0] = reader.ReadSingle();
                data.StarsColor2.Data[1] = reader.ReadSingle();
                data.StarsColor2.Data[2] = reader.ReadSingle();

                data.MaxRings = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.Data[j * data.Width + i] = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.AvoidSearch[j * data.Width + i] = reader.ReadByte();
            }
            else
            {

                data = new GameData(version, reader.ReadUInt32());

                MessageBox.Show("This file is encoded in an old format. It will be converted to version #200",
                    "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);

                uint textureLenght = reader.ReadUInt32();
                uint bumpMapLenght = reader.ReadUInt32();

                data.FloorRenderingMode = RM_TEXTURE;
                data.BumpMappingEnabled = AS_YES;

                data.Texture = new String(reader.ReadChars((int)textureLenght));
                data.BumpMap = new String(reader.ReadChars((int)bumpMapLenght));

                data.StartPoint.Data[0] = reader.ReadInt32();
                data.StartPoint.Data[1] = reader.ReadInt32();

                data.StartDirection.Data[0] = reader.ReadInt32();
                data.StartDirection.Data[1] = reader.ReadInt32();

                data.EmeraldColor.Data[0] = reader.ReadSingle();
                data.EmeraldColor.Data[1] = reader.ReadSingle();
                data.EmeraldColor.Data[2] = reader.ReadSingle();

                data.MaxRings = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.Data[j * data.Width + i] = reader.ReadUInt32();

                for (int j = 0; j < data.Depth; j++)
                    for (int i = 0; i < data.Width; i++)
                        data.AvoidSearch[j * data.Width + i] = reader.ReadByte();

            }

            reader.Close();

            return data;
        }

        public static void StoreData(GameData data, String fileName)
        {
            FileStream fs = new FileStream(fileName, FileMode.Create);
            BinaryWriter writer = new BinaryWriter(fs);

            writer.Write(VERSION_200);

            writer.Write(data.Width);
            writer.Write(data.Depth);

            writer.Write(data.FloorRenderingMode);
            writer.Write(data.BumpMappingEnabled);

            writer.Write((uint)data.Texture.Length);
            writer.Write((uint)data.BumpMap.Length);

            writer.Write(data.Texture.ToCharArray());
            writer.Write(data.BumpMap.ToCharArray());

            writer.Write(data.StartPoint.Data[0]);
            writer.Write(data.StartPoint.Data[1]);

            writer.Write(data.StartDirection.Data[0]);
            writer.Write(data.StartDirection.Data[1]);

            writer.Write(data.EmeraldColor.Data[0]);
            writer.Write(data.EmeraldColor.Data[1]);
            writer.Write(data.EmeraldColor.Data[2]);

            writer.Write(data.CheckerColor1.Data[0]);
            writer.Write(data.CheckerColor1.Data[1]);
            writer.Write(data.CheckerColor1.Data[2]);

            writer.Write(data.CheckerColor2.Data[0]);
            writer.Write(data.CheckerColor2.Data[1]);
            writer.Write(data.CheckerColor2.Data[2]);

            writer.Write(data.SkyBackcolor1.Data[0]);
            writer.Write(data.SkyBackcolor1.Data[1]);
            writer.Write(data.SkyBackcolor1.Data[2]);

            writer.Write(data.SkyBackcolor2.Data[0]);
            writer.Write(data.SkyBackcolor2.Data[1]);
            writer.Write(data.SkyBackcolor2.Data[2]);

            writer.Write(data.StarsColor1.Data[0]);
            writer.Write(data.StarsColor1.Data[1]);
            writer.Write(data.StarsColor1.Data[2]);

            writer.Write(data.StarsColor2.Data[0]);
            writer.Write(data.StarsColor2.Data[1]);
            writer.Write(data.StarsColor2.Data[2]);

            writer.Write(data.MaxRings);

            for (int j = 0; j < data.Depth; j++)
                for (int i = 0; i < data.Width; i++)
                    writer.Write(data.GetValueAt(i, j));

            for (int j = 0; j < data.Depth; j++)
                for (int i = 0; i < data.Width; i++)
                    writer.Write(data.GetAvoidSearchAt(i, j));

            writer.Close();

        }
    }

    class VecGeneric<T>
    {
        private int size;
        private T[] data;

        public int Size
        {
          get { return size; }
        }

        public T[] Data
        {
            get { return data; }
            set { data = value; }
        }

        public VecGeneric(int size) {
            this.size = size;
            this.data = new T[this.size];
        }
    }
     
}
