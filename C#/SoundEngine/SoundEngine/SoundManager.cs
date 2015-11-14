using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SoundEngine
{
    public class SoundManager
    {

        internal const UInt32 BASS_FX_FREESOURCE = 0x10000;

        internal const UInt32 BASS_STREAM_DECODE = 0x200000;

        internal const UInt32 BASS_BASS_ATTRIB_TEMPO = 0x10000;
        internal const UInt32 BASS_ATTRIB_VOL = 2;

        [DllImport("bass.dll")]
        internal static extern bool BASS_Init(Int32 device, UInt32 frequency, UInt32 flags, Int32 hwnd, Int32 guid);

        [DllImport("bass.dll")]
        internal static extern UInt32 BASS_StreamCreateFile(bool mem, string fileName, UInt64 offset, UInt64 length, UInt32 flags);

        [DllImport("bass.dll")]
        internal static extern bool BASS_StreamFree(UInt32 stream);

        [DllImport("bass.dll")]
        internal static extern bool BASS_ChannelPlay(UInt32 stream, bool restart);

        [DllImport("bass.dll")]
        internal static extern bool BASS_ChannelPause(UInt32 stream);

        [DllImport("bass.dll")]
        internal static extern Int32 BASS_ErrorGetCode();

        [DllImport("bass.dll")]
        internal static extern bool BASS_ChannelSetAttribute(UInt32 stream, UInt32 attr, float value);

        [DllImport("bass_fx.dll")]
        internal static extern UInt32 BASS_FX_TempoCreate(UInt32 stream, UInt32 flags);

        internal static void CheckForErrors()
        {
            int errorCode = BASS_ErrorGetCode();
            if(errorCode != 0)
                throw new Exception("An error has occurred. Code: " + errorCode);
        }

        public static void Initialize()
        {
            BASS_Init(1, 44100, 2048, 0, 0);
            CheckForErrors();
        }

        public static SoundStream FromFile(string fileName)
        {
            return new SoundStream(fileName, false);
        }

        public static SoundStream FromFile(string fileName, bool tempo)
        {
            return new SoundStream(fileName, tempo);
        }

    }

    public class SoundStream
    {
        private int _volume;
        private float _tempo;
        private bool _tempoStream;
        private UInt32 _stream;

        internal SoundStream(string fileName, bool tempoStream)
        {
            _tempoStream = tempoStream;

            if (tempoStream)
            {
                _stream = SoundManager.BASS_StreamCreateFile(false, fileName, 0, 0, SoundManager.BASS_STREAM_DECODE);
                _stream = SoundManager.BASS_FX_TempoCreate(_stream, SoundManager.BASS_FX_FREESOURCE);
            }
            else
            {
                _stream = SoundManager.BASS_StreamCreateFile(false, fileName, 0, 0, 0);
            }

            Tempo = 0.0F;
            Volume = 100;

            SoundManager.CheckForErrors();
        }

        public int Volume
        {
            get { return _volume; }
            set
            {
                _volume = value;
                SoundManager.BASS_ChannelSetAttribute(_stream, SoundManager.BASS_ATTRIB_VOL, _volume / 100.0F);
                SoundManager.CheckForErrors();
            }
        }

        public float Tempo
        {
            get { return _tempo; }
            set
            {
                if (!_tempoStream)
                    throw new Exception("This is not a tempo stream");

                _tempo = value;

                SoundManager.BASS_ChannelSetAttribute(_stream, SoundManager.BASS_BASS_ATTRIB_TEMPO, _tempo);

                SoundManager.CheckForErrors();

            }
        }

        public void Play(bool restart)
        {
            SoundManager.BASS_ChannelPlay(_stream, restart);
            SoundManager.CheckForErrors();
        }

        public void Pause()
        {
            SoundManager.BASS_ChannelPause(_stream);
            SoundManager.CheckForErrors();
        }

        ~SoundStream()
        {
            SoundManager.BASS_StreamFree(_stream);
            SoundManager.CheckForErrors();
        }
    }
}
