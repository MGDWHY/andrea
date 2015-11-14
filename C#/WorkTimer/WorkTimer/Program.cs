using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace WorkTimer
{
    static class Program
    {

        public static string DATA_FILE = Path.GetDirectoryName(Application.ExecutablePath) + "\\timers.dat";

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }


        public static void SaveData(List<WorkTimer> data, string fileName)
        {

            try
            {
                FileStream stream = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.None);
                BinaryFormatter formatter = new BinaryFormatter();
                formatter.Serialize(stream, data);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred while writing: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public static List<WorkTimer> LoadData(string fileName)
        {
            try
            {
                FileStream stream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.None);
                BinaryFormatter formatter = new BinaryFormatter();
                return (List<WorkTimer>)formatter.Deserialize(stream);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An error occurred while reading: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return null;
            }
        }
    }
}
