#pragma once

#include <vector>
#include "vecmath.h"

using namespace std;

namespace glutils {

	class Face {
	public:
		string getMaterial();
		string getGroup();

		int GetVertexIndex(int vertex);
		int GetNormalIndex(int vertex);
		int GetTexCoordIndex(int vertex);

		int GetVerticesCount();

	private:

		Face();

		int verticesCount;

		string group;
		string material;

		vector<int> vertexIndices;
		vector<int> normalIndices;
		vector<int> texCoordIndices;

		void addVertex(int iVertex);
		void addVertex(int iVertex, int iTexCoords);
		void addVertex(int iVertex, int iTexCoords, int iNormal);

		void setMaterial(string m);
		void setGroup(string g);

		friend class ObjectFile;
	};
}