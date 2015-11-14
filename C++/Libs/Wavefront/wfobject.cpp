#include "WFObject.h"

namespace WaveFront {

	const string WFObject::T_VERTEX = "v";
	const string WFObject::T_NORMAL = "vn";
	const string WFObject::T_TEXCOORDS = "vt";
	const string WFObject::T_FACE = "f";
	const string WFObject::T_GROUP = "g";
	const string WFObject::T_MATERIAL = "usemtl";
	const string WFObject::T_COMMENT = "#";

	const unsigned int WFObject::VBO_VERTEX = 0;
	const unsigned int WFObject::VBO_NORMAL = 1;
	const unsigned int WFObject::VBO_TEXCOORD = 2;

	WFObject::WFObject() {
		this->vertices = vector<vec3>();
		this->normals = vector<vec3>();
		this->texCoords = vector<vec2>();
		this->faces = vector<WFFace*>();
	}

	WFObject::~WFObject() {
		this->vertices.clear();
		this->normals.clear();
		this->texCoords.clear();
		this->faces.clear();
	}


	vector<vec3> * WFObject::GetVertices() {
		return &(this->vertices);
	}

	vector<vec3> * WFObject::GetNormals() {
		return &(this->normals);
	}

	vector<vec2> * WFObject::GetTexCoords() {
		return &(this->texCoords);
	}

	vector<WFFace*> * WFObject::GetFaces() {
		return &(this->faces);
	}

	vector<WFFace*> * WFObject::GetFaces(string material) {
		vector<WFFace*> * res = new vector<WFFace*>;

		for(int i = 0; i < this->faces.size(); i++) {
			if(material.compare(faces[i]->GetMaterial()) == 0)
				res->push_back(this->faces[i]);
		}

		return res;
	}

	WFObject * WFObject::FromFile(const string &fileName) {
		WFObject *obj = new WFObject();
		ifstream file;
		string currentGroup = "", currentMaterial = "";
		int skippedLines = 0; // for debug
		file.open(fileName);

		if(!file.is_open()) {
			#ifdef GL_UTILS_LOG_ENABLED
				Logger::getDefaultLogger()->writeMessage(2, "ObjectFile::load()", "Can't open file: " + fileName);
			#endif
			return NULL;
		}

		#ifdef GL_UTILS_LOG_ENABLED
			Logger::getDefaultLogger()->writeMessage(0, "ObjectFile::load()", "------------------------------------");
			Logger::getDefaultLogger()->writeMessage(0, "ObjectFile::load()", "Loading file: " + fileName);
		#endif

		while(file.good()) {

			float x, y, z;
			char ch;
			string line, op, temp;

			getline(file, line);
			stringstream ss(line);

			ss >> op;
			
			if(op.compare(T_VERTEX) == 0) { // Vertex
				ss >> x >> y >> z;
				obj->vertices.push_back(vec3(x,y,z));
			} else if(op.compare(T_NORMAL) == 0) { // Vertex normal
				ss >> x >> y >> z;
				obj->normals.push_back(vec3(x,y,z));
			} else if(op.compare(T_TEXCOORDS) == 0) { // Texture coordinates
				ss >> x >> y;
				obj->texCoords.push_back(vec2(x,y));
			} else if(op.compare(T_FACE) == 0) { // Face
				WFFace *face = new WFFace();
				while(!ss.eof()) {

					ss >> temp;
					stringstream ss2(temp);

					int v = 0, n = 0, t = 0;
					int ind1 = temp.find("/", 0);
					int ind2 = temp.find("/", ind1 + 1);

					ss2 >> v; // Vertex

					if(ind1 > 0) {
						if(ind2 < 0) { // v|t
							ss2 >> ch >> t; // Texture
						} else if(ind2 == ind1 + 1) { // v||n
							ss2 >> ch >> ch >> n;
						} else {
							ss2 >> ch >> t >> ch >> n; // v|t|n
						}
					}

					/*
					if(ind1 > 0)
						if(ind2 < 0)
							ss2 >> ch >> t; // Texture
						else
							ss2 >> ch >> t >> ch >> n; // Normal and texture*/

					face->AddVertex(v-1, t-1, n-1);
					face->SetGroup(currentGroup);
					face->SetMaterial(currentMaterial);
				}

				obj->faces.push_back(face);

			} else if(op.compare(T_GROUP) == 0) { // Groups
				ss >> currentGroup;
			} else if(op.compare(T_MATERIAL) == 0) { // Materials
				ss >> currentMaterial;
			} else if(op.compare(T_COMMENT) == 0) { // Comment
				#ifdef GL_UTILS_LOG_ENABLED
					Logger::getDefaultLogger()->writeMessage(0, "ObjectFile::load()", line);
				#endif			
			} else { // Unknown token
				#ifdef GL_UTILS_LOG_ENABLED
					skippedLines++;
				#endif		
			}
		}

		file.close();

		#ifdef GL_UTILS_LOG_ENABLED
			stringstream msgss;
			
			msgss	<< "------------------------------------" << endl
					<< "File loaded: " + fileName << endl
					<< "Skipped lines (unknown tokens): " << skippedLines << endl
					<< "------------------------------------" << endl
					<< "Total vertices: " << obj->vertices.size() << endl
					<< "Total normals: " << obj->normals.size() << endl
					<< "Total texcoords: " << obj->texCoords.size() << endl
					<< "Total faces: " << obj->faces.size();

			Logger::getDefaultLogger()->writeMessageStream(0, "ObjectFile::load()", msgss);

		#endif

		return obj;
	}
}