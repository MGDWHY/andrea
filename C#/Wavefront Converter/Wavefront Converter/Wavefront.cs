using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace WavefrontConverter
{
    public class WFObject
    {

        private static readonly string T_COMMENT = "#";

        private static readonly string T_VERTEX = "v";
        private static readonly string T_NORMAL = "vn";
        private static readonly string T_TEXCOORD = "vt";
        private static readonly string T_FACE = "f";
        private static readonly string T_USEMATERIAL = "usemtl";

        private static readonly string LOG_SEPARATOR = "<div style=\"border-bottom: 1px solid #000000\"></div>";

        private static readonly string LOG_ERROR = "#ff0000";
        private static readonly string LOG_WARNING = "#aaaa00";
        private static readonly string LOG_INFO = "#000000";
        private static readonly string LOG_COMMENT = "#006600";

        private static readonly string DEFAULT_GROUP_ID = "default";

        private List<WFVertex> vertices;
        private List<WFNormal> normals;
        private List<WFTexCoord> texCoords;
        private Dictionary<string, WFFaceGroup> faceGroups;

        private string loadLog;

        private WFObject()
        {
            vertices = new List<WFVertex>();
            normals = new List<WFNormal>();
            texCoords = new List<WFTexCoord>();
            faceGroups = new Dictionary<string, WFFaceGroup>();
            faceGroups.Add(DEFAULT_GROUP_ID, new WFFaceGroup());
        }

        public List<WFVertex> Vertices
        {
            get { return vertices; }
        }

        public List<WFNormal> Normals
        {
            get { return normals; }
        }

        public List<WFTexCoord> TexCoords
        {
            get { return texCoords; }
        }

        public WFFaceGroup this[int index]
        {
            get { return faceGroups.ElementAt(index).Value; }
        }

        public WFFaceGroup this[string groupID]
        {
            get { return faceGroups[groupID]; }
        }

        public string[] FaceGroups
        {
            get { return faceGroups.Keys.ToArray(); }
        }

        public string LoadLog
        {
            get { return loadLog; }
        }

        public static WFObject Load(string filename)
        {
            int linesSkipped = 0, unknownTokens = 0, faces = 0;
            StreamReader reader = new StreamReader(filename);
            WFObject result = new WFObject();
            string currentGroup = DEFAULT_GROUP_ID;
            string[] sep1 = { " " };
            string[] sep2 = { "/" };

            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine().Trim();

                string[] tokens = line.Split(sep1, StringSplitOptions.RemoveEmptyEntries);

                if (tokens.Length == 0)
                {
                    linesSkipped++;
                    continue;
                }

                string command = tokens[0].ToLower();

                if (command.Equals(T_VERTEX)) // new vertex
                {
                    result.vertices.Add(new WFVertex(float.Parse(tokens[1]), float.Parse(tokens[2]), float.Parse(tokens[3])));
                }
                else if (command.Equals(T_NORMAL)) // new normal
                {
                    result.normals.Add(new WFNormal(float.Parse(tokens[1]), float.Parse(tokens[2]), float.Parse(tokens[3])));
                }
                else if (command.Equals(T_TEXCOORD)) // new texcoord
                {
                    result.texCoords.Add(new WFTexCoord(float.Parse(tokens[1]), float.Parse(tokens[2])));
                }
                else if(command.Equals(T_FACE)) { // new face

                    WFFace face = new WFFace(); // create the new face

                    for (int i = 1; i < tokens.Length; i++) // get the vertices
                    {
                        string faceDescr = tokens[i];

                        string[] components = faceDescr.Split(sep2, StringSplitOptions.None);

                        int v = int.Parse(components[0]) - 1;
                        int vt = -1;
                        int vn = -1;

                        if (components.Length == 2) // vi/vt
                        {
                            vt = int.Parse(components[1]) - 1;
                        }
                        else if (components.Length == 3) // vi/(vt)/vn
                        {
                            vt = components[1].Equals(string.Empty) ? -1 : int.Parse(components[1]) - 1;
                            vn = int.Parse(components[2]) - 1;
                        }

                        face.AddVertex(v,vn, vt); // add the vertex to the face
                    }

                    faces++;

                    // add the face to the current group
                    if (result.faceGroups.ContainsKey(currentGroup))
                    {
                        result.faceGroups[currentGroup].AddFace(face);
                    }
                    else
                    {
                        WFFaceGroup grp = new WFFaceGroup(currentGroup);

                        grp.AddFace(face);

                        result.faceGroups.Add(currentGroup, grp);
                    }
                }
                else if (command.Equals(T_USEMATERIAL))
                {
                    currentGroup = tokens[1];
                }
                else if (command.Equals(T_COMMENT))
                {
                    result.WriteLog(line, LOG_COMMENT);
                }
                else
                {
                    unknownTokens++;
                }

                Thread.Yield();
                
            }

            result.WriteLogSeparator();
            result.WriteLog("Loading done!", LOG_INFO);
            result.WriteLogSeparator();
            result.WriteLog("Vertices: " + result.vertices.Count, LOG_INFO);
            result.WriteLog("Normals: " + result.normals.Count, LOG_INFO);
            result.WriteLog("TexCoords: " + result.texCoords.Count, LOG_INFO);
            result.WriteLog("Faces: " + faces, LOG_INFO);
            result.WriteLog("Groups: " + result.faceGroups.Count, LOG_INFO);
            result.WriteLogSeparator();
            result.WriteLog("Empty lines: " + linesSkipped, LOG_INFO);
            result.WriteLog("Unknown tokens: " + unknownTokens, LOG_WARNING);

            return result;
        }

        private void WriteLogSeparator()
        {
            WriteLog(LOG_SEPARATOR, LOG_INFO);
        }

        private void WriteLog(string str, string color)
        {
            loadLog += "<font color=\"" + color + "\">" +  str + "</font></br>";
        }

    }

    public class WFFaceGroup
    {
        private string materialName;
        private List<WFFace> faces;


        public int FaceCount
        {
            get { return faces.Count; }
        }

        public string MaterialName
        {
            get { return materialName; }
        }

        public WFFace this[int index]
        {
            get { return faces.ElementAt(index); }
        }

        public WFFaceGroup() : this(string.Empty)
        {
        }

        public WFFaceGroup(string materialName)
        {
            this.materialName = materialName;
            this.faces = new List<WFFace>();
        }



        public void AddFace(WFFace face)
        {
            faces.Add(face);
        }


    }

    public class WFFace
    {
        private List<int> vertices;
        private List<int> normals;
        private List<int> texCoords;

        public int VerticesCount
        {
            get { return vertices.Count; }
        }

        public int[] this[int index]
        {
            get { return new int[] { vertices.ElementAt(index), normals.ElementAt(index), texCoords.ElementAt(index) }; }
        }

        public WFFace()
        {
            vertices = new List<int>();
            normals = new List<int>();
            texCoords = new List<int>();
        }

        public void AddVertex(int iVertex)
        {
            AddVertex(iVertex, -1, -1);
        }

        public void AddVertex(int iVertex, int iTexCoord)
        {
            AddVertex(iVertex, -1, iTexCoord);
        }

        public void AddVertex(int iVertex, int iNormal, int iTexCoord)
        {
            vertices.Add(iVertex);
            normals.Add(iNormal);
            texCoords.Add(iTexCoord);
        }
    }
    


    public struct WFVertex
    {
        public float X, Y, Z;

        public WFVertex(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }
    }

    public struct WFNormal
    {
        public float Nx, Ny, Nz;

        public WFNormal(float nx, float ny, float nz)
        {
            this.Nx = nx;
            this.Ny = ny;
            this.Nz = nz;
        }
    }

    public struct WFTexCoord
    {
        public float Tu, Tv;

        public WFTexCoord(float tu, float tv)
        {
            this.Tu = tu;
            this.Tv = tv;
        }
    }

    public abstract class WFFileConverter
    {

        public abstract string FormatName
        {
            get;
        }

        /* File extension */
        public abstract string FileExtension
        {
            get;
        }

        /* Writes an obj file */
        public abstract void Write(WFObject inputObject, FileStream outStream);

        /* Reads an obj file */
        public abstract WFObject Read(FileStream inStream);
    }
}
