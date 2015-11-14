#include "WFFace.h"

namespace WaveFront {

	WFFace::WFFace() {
		this->verticesCount = 0;
	}

	int WFFace::GetVertexIndex(int vertex) { return this->vertexIndices[vertex];}
	int WFFace::GetNormalIndex(int vertex) { return this->normalIndices[vertex];}
	int WFFace::GetTexCoordIndex(int vertex) { return this->texCoordIndices[vertex];}

	int WFFace::GetVerticesCount() { return this->verticesCount;}

	bool WFFace::ContainsVertexIndex(int vertex) {
		for(int i = 0; i < verticesCount; i++)
			if(this->vertexIndices[i] == vertex)
				return true;

		return false;
	}

	void WFFace::AddVertex(int iVertex, int iTexCoords, int iNormal) {
		this->vertexIndices.push_back(iVertex);
		this->normalIndices.push_back(iNormal);
		this->texCoordIndices.push_back(iTexCoords);
		this->verticesCount++;
	}

	void WFFace::AddVertex(int iVertex, int iTexCoords) {
		this->AddVertex(iVertex, iTexCoords, -1);
	}

	void WFFace::AddVertex(int iVertex) {
		this->AddVertex(iVertex, -1, -1);
	}

	void WFFace::SetGroup(string g) { group = g; }
	void WFFace::SetMaterial(string m) { material = m;}

	string WFFace::GetGroup() { return group;}
	string WFFace::GetMaterial() { return material; }
}