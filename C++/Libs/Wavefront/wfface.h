#pragma once

#include <vector>
#include "vecmath.h"

using namespace std;

namespace WaveFront {

	class WFFace {
	public:
		string GetMaterial();
		string GetGroup();

		int GetVertexIndex(int vertex);
		int GetNormalIndex(int vertex);
		int GetTexCoordIndex(int vertex);

		int GetVerticesCount();

		bool ContainsVertexIndex(int vertex);

	private:

		WFFace();

		int verticesCount;

		string group;
		string material;

		vector<int> vertexIndices;
		vector<int> normalIndices;
		vector<int> texCoordIndices;

		void AddVertex(int iVertex);
		void AddVertex(int iVertex, int iTexCoords);
		void AddVertex(int iVertex, int iTexCoords, int iNormal);

		void SetMaterial(string m);
		void SetGroup(string g);

		friend class WFObject;
	};
}