using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace WavefrontConverter
{
    /*
     * Android 3D Object converter
     * Reads and writes obj file to Android 3D file
     * a3d file format:
     *  header: version(int), numshadinggroups(int)
     *  numshadinggroups(n): idlen(int),id(idlen bytes), numvertices(int), vertices
     *  vertex: x y z nx ny nz tu tv (all floats) -> if same values are not available they'll be filled with 0.0f
     *  NB: LITTLE ENDIAN
     */
    public class Android3DObjectConverter : WFFileConverter
    {

        private static readonly int CURRENT_VERSION = 100;

        private static readonly int VERTEX_SIZE_IN_WORDS = 8;

        public override string FormatName
        {
            get { return "Android 3D Object"; }
        }

        public override string FileExtension
        {
            get { return "a3o"; }
        }

        public override void Write(WFObject inputObject, FileStream outStream)
        {
            BinaryWriter writer = new BinaryWriter(outStream);

            string[] faceGroupIDs = inputObject.FaceGroups;

            int shadingGroupsCount = inputObject.FaceGroups.Length;

            /* Remove the empty groups from the count */
            for (int i = 0; i < inputObject.FaceGroups.Length; i++)
                if (inputObject[i].FaceCount == 0)
                    shadingGroupsCount--;

            // write header;
            writer.Write(CURRENT_VERSION);
            writer.Write(shadingGroupsCount);

            // write face groups

            foreach (string gID in faceGroupIDs)
            {
                WFFaceGroup currentGroup = inputObject[gID];
                List<float> vertices = new List<float>(); //store all the vertices in this group

                for(int fi = 0; fi < currentGroup.FaceCount; fi++) // foreach face
                {
                    WFFace currentFace = currentGroup[fi];

                    for (int vi = 0; vi < currentFace.VerticesCount; vi++) // foreach vertex
                    {
                        int[] indexes = currentFace[vi];

                        WFVertex vertex = inputObject.Vertices[indexes[0]];
                        WFNormal normal = inputObject.Normals[indexes[1]];
                        WFTexCoord texCoord = inputObject.TexCoords[indexes[2]];

                        vertices.Add(vertex.X);
                        vertices.Add(vertex.Y);
                        vertices.Add(vertex.Z);
                        vertices.Add(normal.Nx);
                        vertices.Add(normal.Ny);
                        vertices.Add(normal.Nz);
                        vertices.Add(texCoord.Tu);
                        vertices.Add(texCoord.Tv);

                    }
                }

                byte[] verticesData = new byte[vertices.Count * sizeof(float)];

                System.Buffer.BlockCopy(vertices.ToArray(), 0, verticesData, 0, verticesData.Length);
                
                byte[] gIDbin = System.Text.Encoding.ASCII.GetBytes(gID);

                // if no vertices in this group, let's not write it
                if (vertices.Count == 0)
                    continue;

                // write face group header
                writer.Write(gIDbin.Length);
                writer.Write(gIDbin);
                writer.Write(vertices.Count / VERTEX_SIZE_IN_WORDS);

                // write veriices data
                writer.Write(verticesData);
            }

        }

        public override WFObject Read(System.IO.FileStream inStream)
        {
            throw new NotImplementedException("This methoed is not available");
        }
    }
}
