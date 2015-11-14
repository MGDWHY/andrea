#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "binio.h"
#include "vecmath.h"
#include "wfo.h"

using namespace WaveFront;
using namespace Vecmath;
using namespace BinaryIO;
using namespace std;

namespace ExtendedBinary3DObject {

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TexCoord;
		Vertex(vec3 position, vec3 normal, vec2 texCoord);
		Vertex();
	};

	struct Material {
		vec4 Ambient;
		vec4 Diffuse;
		vec4 Specular;
		vec4 Emissive;
		float Shininess;
		Material();
	};

	struct ShadingGroup {
		Material Material;
		int VerticesCount;
		Vertex * Vertices;
		~ShadingGroup();
	};

	struct XBO {
		int Version;
		int ShadingGroupCount;
		ShadingGroup * ShadingGroups;
		~XBO();
	};

	XBO * FromFile(string fileName);

	void ToFile(XBO * xbo, string fileName);
	void ToFile(WFObject * wfo, string fileName);
	void ToFile(WFObject * wfo, WFMaterialLibrary * wfmatlib, string fileName);
}