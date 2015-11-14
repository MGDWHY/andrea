#pragma once

#include <fstream>
#include <sstream>
#include "vecmath.h"
#include "Logger.h"
#include "Face.h"
#include "MaterialLibrary.h"
#include "VertexArrayObject.h"


using namespace Vecmath;
using namespace std;

namespace glutils {

	class ObjectFile {
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
		vector<Face*> faces;

		// Materials
		MaterialLibrary * materialLibrary;

		ObjectFile();

	public:

		~ObjectFile();

		static ObjectFile* load(const string &fileName);

		// Display Lists: DEPRECATED
		// Display lists will be no longer available on opengl 4. Use VertexArrayObjects
		unsigned int createDisplayList();
		unsigned int createDisplayList(const string *group, const string *material);

		vector<vec3> * GetVertices();
		vector<vec3> * GetNormals();
		vector<vec2> * GetTexCoords();

		vector<Face> * GetFaces();
		vector<Face> * GetFaces(string material);


		// Vertex Buffer Obejcts
		VertexArrayObject * createVertexArrayObject(string *group, string *material, unsigned int bufferUsage);

	};

}