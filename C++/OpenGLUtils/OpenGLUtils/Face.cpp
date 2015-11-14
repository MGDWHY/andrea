#include "Face.h"

namespace glutils {

	Face::Face() {
		this->verticesCount = 0;
	}


	int Face::GetVertexIndex(int vertex) { return this->vertexIndices[vertex];}
	int Face::GetNormalIndex(int vertex) { return this->normalIndices[vertex];}
	int Face::GetTexCoordIndex(int vertex) { return this->texCoordIndices[vertex];}

	int Face::GetVerticesCount() { return this->verticesCount;}

	void Face::addVertex(int iVertex, int iTexCoords, int iNormal) {
		this->vertexIndices.push_back(iVertex);
		this->normalIndices.push_back(iNormal);
		this->texCoordIndices.push_back(iTexCoords);
		this->verticesCount++;
	}

	void Face::addVertex(int iVertex, int iTexCoords) {
		addVertex(iVertex, iTexCoords, -1);
	}

	void Face::addVertex(int iVertex) {
		addVertex(iVertex, -1, -1);
	}

	void Face::setGroup(string g) { group = g; }
	void Face::setMaterial(string m) { material = m;}

	string Face::getGroup() { return group;}
	string Face::getMaterial() { return material; }
}