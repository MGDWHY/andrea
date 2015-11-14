using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace Sonic3SSSectionDecoder
{
    [Serializable]
    class SectionDecoder
    {

        public static readonly int SECTION_WIDTH = 16;
        public static readonly int SECTION_HEIGHT = 16;

        public static readonly uint SECTION_ITEM_UNKNOWN = 999;

        private static readonly int BMP_ITEM_SIZE = 10;
        private static readonly int BMP_OFFSET_X = 2;
        private static readonly int BMP_OFFSET_Y = 2;
        private static Bitmap[] stageItems;
        
        static SectionDecoder() {

            SectionDecoder.stageItems = new Bitmap[6];

            for (int i = 0; i < 6; i++)
            {
                SectionDecoder.stageItems[i] = new Bitmap(Image.FromFile("images/items/" + i + ".png"));
            }
        }

        private bool decoded;
        private int id;
        private Bitmap sectionBitmap;
        private DecodedSectionData data;

        public int Id
        {
            get { return id; }
        }

        public Bitmap SectionBitmap
        {
            get { return sectionBitmap; }
        }

        public DecodedSectionData Data
        {
            get { return data; }
        }

        public SectionDecoder(String sectionFile, int id)
        {
            this.decoded = false;
            this.id = id;
            this.sectionBitmap = new Bitmap(Image.FromFile(sectionFile));
            this.data = new DecodedSectionData();
        }

        public void Decode(bool force)
        {
            if (this.decoded && !force)
                return;

            for(int x = 0; x < SectionDecoder.SECTION_WIDTH; x++)
                for (int y = 0; y < SectionDecoder.SECTION_HEIGHT; y++)
                {
                    int offsetX = SectionDecoder.BMP_OFFSET_X + x * BMP_ITEM_SIZE;
                    int offsetY = SectionDecoder.BMP_OFFSET_Y + y * BMP_ITEM_SIZE;

                    int obj = this.GetObjectAt(offsetX, offsetY);

                    if (obj >= 0)
                        this.data.SetValueAt((uint)obj, x, y);
                    else
                        this.data.SetValueAt(SectionDecoder.SECTION_ITEM_UNKNOWN, x, y);
                }

            this.decoded = true;
        }

        private int GetObjectAt(int offsetX, int offsetY)
        {
            for (int obj = 0; obj < 6; obj++)
            {
                bool equal = true;

                for(int x = 0; x < 8; x++)
                    for(int y = 0; y < 8; y++)
                        if(SectionDecoder.stageItems[obj].GetPixel(x, y) != this.sectionBitmap.GetPixel(x + offsetX, y + offsetY)) 
                        {
                            equal = false;
                            break;
                        }

                if (equal)
                    return obj;
            }

            return -1;
        }

        public override string ToString()
        {
            return "Section " + this.id;
        }
    }

    [Serializable]
    class DecodedSectionData
    {

        private uint maxRings;

        private uint[] data;

        private byte[] avoidSearch;

        public DecodedSectionData()
        {
            this.maxRings = 0;
            this.data = new uint[SectionDecoder.SECTION_WIDTH * SectionDecoder.SECTION_HEIGHT];
            this.avoidSearch = new byte[SectionDecoder.SECTION_WIDTH * SectionDecoder.SECTION_HEIGHT];

            for (int i = 0; i < this.data.Length; i++)
            {
                this.data[i] = 0;
                this.avoidSearch[i] = 0;
            }
        }

        public uint MaxRings
        {
            get { return maxRings; }
            set { maxRings = value; }
        }

        public void SetAvoidSearchAt(byte value, int x, int y)
        {
            this.avoidSearch[y * SectionDecoder.SECTION_WIDTH + x] = value;
        }

        public byte GetAvoidSearchAt(int x, int y)
        {
            return this.avoidSearch[y * SectionDecoder.SECTION_WIDTH + x];
        }

        public void SetValueAt(uint value, int x, int y)
        {
            this.data[y * SectionDecoder.SECTION_WIDTH + x] = value;
        }

        public uint GetValueAt(int x, int y)
        {
            return this.data[y * SectionDecoder.SECTION_WIDTH + x];
        }

        public void Write(BinaryWriter writer)
        {
            writer.Write(this.maxRings);

            for (int i = 0; i < this.data.Length; i++ )
                writer.Write(this.data[i]);

            for (int i = 0; i < this.avoidSearch.Length; i++)
                writer.Write(this.avoidSearch[i]);
        }
    }
}
