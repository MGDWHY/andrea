#pragma once

#include <fstream>
#include <sstream>
#include "vecmath.h"
#include "WFFace.h"
#include "WFMaterialLibrary.h"

using namespace Vecmath;
using namespace std;

namespace WaveFront {

	class WFObject {
	private:

		// File tokens
		static const string T_VERTEX;
		static const string T_NORMAL;
		static const string T_TEXCOORDS;
		static const string T_FACE;
		static const string T_GROUP;
		static const string T_MATERIAL;
		static const string T_COMMENT;

		// Vertex Buffer attrbutes
		static const unsigned int VBO_VERTEX;
		static const unsigned int VBO_NORMAL;
		static const unsigned int VBO_TEXCOORD;

		// Vertices, normals and texCoords
		vector<vec3> vertices, normals;
		vector<vec2> texCoords;

		// Mesh faces
		vector<WFFace*> faces;

		// Materials
		WFMaterialLibrary * materialLibrary;

		WFObject();

	public:

		~WFObject();

		static WFObject* FromFile(const string &fileName);

		vector<vec3> * GetVertices();
		vector<vec3> * GetNormals();
		vector<vec2> * GetTexCoords();

		vector<WFFace*> * GetFaces();
		vector<WFFace*> * GetFaces(string material);

	};

}