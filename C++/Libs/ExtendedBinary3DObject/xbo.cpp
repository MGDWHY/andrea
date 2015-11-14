#include "xbo.h"

namespace ExtendedBinary3DObject {

	ShadingGroup * ReadShadingGroup(ifstream& is);
	void WriteShadingGroup(ofstream& os, ShadingGroup * sg);

	XBO::~XBO() {
		delete[] this->ShadingGroups;
	}

	Material::Material() {
		this->Shininess = 0.0f;
	}

	Vertex::Vertex(vec3 position, vec3 normal, vec2 texCoord) {
		this->Position = position;
		this->Normal = normal;
		this->TexCoord = texCoord;
	}

	Vertex::Vertex() {}

	ShadingGroup::~ShadingGroup() {
		delete[] this->Vertices;
	}

	XBO * FromFile(string fileName) {
		ifstream is;

		is.open(fileName.c_str(), ios_base::binary);
		
		if(!is.good())
			return NULL;

		XBO * result = new XBO();

		result->Version = Read<int>(is);
		result->ShadingGroupCount = Read<int>(is);
		result->ShadingGroups = new ShadingGroup[result->ShadingGroupCount];
		
		for(int i = 0; i < result->ShadingGroupCount; i++)
			result->ShadingGroups[i] = *(ReadShadingGroup(is));

		return result;
	}

	void ToFile(WFObject * wfo, string fileName) {

		XBO * dest = new XBO();

		dest->Version = 100;
		dest->ShadingGroupCount = 1;
		dest->ShadingGroups = new ShadingGroup();

		vector<WFFace*> * faces = wfo->GetFaces();
		vector<Vertex> * vertices = new vector<Vertex>();

		for(int i = 0; i < faces->size(); i++) {
			WFFace *cface = faces->at(i);
			for(int j = 0; j < cface->GetVerticesCount(); j++) {
				vec3 position = wfo->GetVertices()->at(cface->GetVertexIndex(j));
				vec3 normal = wfo->GetNormals()->at(cface->GetNormalIndex(j));
				vec2 texCoord = wfo->GetTexCoords()->at(cface->GetTexCoordIndex(j));
				vertices->push_back(Vertex(position, normal, texCoord));
			}
		}

		dest->ShadingGroups[0].Material = Material();
		dest->ShadingGroups[0].VerticesCount = vertices->size();
		dest->ShadingGroups[0].Vertices = &(vertices->at(0));

		ToFile(dest, fileName);
	}

	void ToFile(WFObject * wfo, WFMaterialLibrary * wfmatlib, string fileName) {
		XBO * dest = new XBO();

		dest->Version = 100;
		dest->ShadingGroupCount = wfmatlib->GetMaterialCount();
		dest->ShadingGroups = new ShadingGroup[dest->ShadingGroupCount];

		for(int k = 0; k < dest->ShadingGroupCount; k++) {

			WFMaterial cmat = wfmatlib->GetMaterial(k);

			vector<WFFace*> * faces = wfo->GetFaces(cmat.GetName());
			vector<Vertex> * vertices = new vector<Vertex>();

			for(int i = 0; i < faces->size(); i++) {
				WFFace *cface = faces->at(i);
				for(int j = 0; j < cface->GetVerticesCount(); j++) {
					vec3 position = wfo->GetVertices()->at(cface->GetVertexIndex(j));
					vec3 normal = wfo->GetNormals()->at(cface->GetNormalIndex(j));
					vec2 texCoord = wfo->GetTexCoords()->at(cface->GetTexCoordIndex(j));
					vertices->push_back(Vertex(position, normal, texCoord));
				}
			}

			dest->ShadingGroups[k].Material.Ambient = vec4(cmat.GetKa(), 1.0f);
			dest->ShadingGroups[k].Material.Diffuse = vec4(cmat.GetKd(), 1.0f);
			dest->ShadingGroups[k].Material.Specular = vec4(cmat.GetKs(), 1.0f);
			dest->ShadingGroups[k].Material.Emissive = vec4();
			dest->ShadingGroups[k].Material.Shininess = 128.0f;

			dest->ShadingGroups[k].VerticesCount = vertices->size();
			dest->ShadingGroups[k].Vertices = &(vertices->at(0));
		}

		ToFile(dest, fileName);
	}

	void ToFile(XBO * xbo, string fileName) {
		ofstream os;

		os.open(fileName, ios_base::binary);

		if(!os.good()) {
			os.close();
			return;
		}

		Write<int>(os, xbo->Version);
		Write<int>(os, xbo->ShadingGroupCount);

		for(int i = 0; i < xbo->ShadingGroupCount; i++)
			WriteShadingGroup(os, &(xbo->ShadingGroups[i]));

		os.close();
	}

	void WriteShadingGroup(ofstream& os, ShadingGroup * sg) {
		Write<Material>(os, sg->Material);
		Write<int>(os, sg->VerticesCount);
		os.write((char*)sg->Vertices, sizeof(Vertex) * sg->VerticesCount);
	}

	ShadingGroup * ReadShadingGroup(ifstream& is) {
		ShadingGroup * result = new ShadingGroup();

		result->Material = Read<Material>(is);
		result->VerticesCount = Read<int>(is);
		result->Vertices = new Vertex[result->VerticesCount];

		is.read((char*)result->Vertices, sizeof(Vertex) * result->VerticesCount);

		return result;
	}

}