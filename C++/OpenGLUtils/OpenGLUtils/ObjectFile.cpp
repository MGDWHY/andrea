#include "ObjectFile.h"

namespace glutils {

	const string ObjectFile::T_VERTEX = "v";
	const string ObjectFile::T_NORMAL = "vn";
	const string ObjectFile::T_TEXCOORDS = "vt";
	const string ObjectFile::T_FACE = "f";
	const string ObjectFile::T_GROUP = "g";
	const string ObjectFile::T_MATERIAL = "usemtl";
	const string ObjectFile::T_COMMENT = "#";

	const unsigned int ObjectFile::VBO_VERTEX = 0;
	const unsigned int ObjectFile::VBO_NORMAL = 1;
	const unsigned int ObjectFile::VBO_TEXCOORD = 2;

	ObjectFile::ObjectFile() {
		this->vertices = vector<vec3>();
		this->normals = vector<vec3>();
		this->texCoords = vector<vec2>();
		this->faces = vector<Face*>();
	}

	ObjectFile::~ObjectFile() {
		this->vertices.clear();
		this->normals.clear();
		this->texCoords.clear();
		this->faces.clear();
	}

	// deprecated (le display list non ci sono su OpenGL 4.0+). Sostituita da createVertexArrayObject
	unsigned int ObjectFile::createDisplayList() {
		return createDisplayList(NULL,NULL);
	}
	// deprecated (come sopra)
	unsigned int ObjectFile::createDisplayList(const string *group, const string *material) {
		unsigned int result = glGenLists(1);
		
		glNewList(result, GL_COMPILE);

		for(unsigned int i = 0; i < faces.size(); i++) {
			
			Face *face = faces[i];

			if(group != NULL && face->getGroup().compare(*group) != 0)
				continue;

			if(material != NULL && face->getMaterial().compare(*material) != 0)
				continue;

			switch(face->GetVerticesCount()) {
				case 3: glBegin(GL_TRIANGLES); break;
				case 4: glBegin(GL_QUADS); break;
				default: glBegin(GL_POLYGON); break;
			}

			for(unsigned int j = 0; j < face->GetVerticesCount(); j++) {

				if(face->GetNormalIndex(j) != -1)
					glNormal3fv(normals[face->GetNormalIndex(j)].xyz);

				if(face->GetTexCoordIndex(j) != -1)
					glTexCoord2fv(texCoords[face->GetTexCoordIndex(j)].xy);

				glVertex3fv(vertices[face->GetVertexIndex(j)].xyz);

			}

			glEnd();

		}

		glEndList();

		return result;
	}

	// TODO da rifare con interleaved array con dati allineati alla memoria (32/64 byte)
	VertexArrayObject * ObjectFile::createVertexArrayObject(string *group, string *material, unsigned int bufferUsage) {	

		vector<float> temp_vertices = vector<float>();
		vector<float> temp_normals = vector<float>();
		vector<float> temp_texCoords = vector<float>();
		unsigned int *indexes;
		
		for(unsigned int i = 0; i < faces.size(); i++) {
			
			Face *face = faces[i];

			if(group != NULL && face->getGroup().compare(*group) != 0)
				continue;

			if(material != NULL && face->getMaterial().compare(*material) != 0)
				continue;

			for(unsigned int j = 0; j < face->GetVerticesCount(); j++) {

				temp_vertices.push_back(vertices[face->GetVertexIndex(j)].x);
				temp_vertices.push_back(vertices[face->GetVertexIndex(j)].y);
				temp_vertices.push_back(vertices[face->GetVertexIndex(j)].z);

				if(face->GetTexCoordIndex(j) != -1) {
					temp_texCoords.push_back(texCoords[face->GetTexCoordIndex(j)].x);
					temp_texCoords.push_back(texCoords[face->GetTexCoordIndex(j)].y);
				} else {
					temp_texCoords.push_back(0.0f);
					temp_texCoords.push_back(0.0f);
				}

				if(face->GetNormalIndex(j) != -1) {
					temp_normals.push_back(normals[face->GetNormalIndex(j)].x);
					temp_normals.push_back(normals[face->GetNormalIndex(j)].y);
					temp_normals.push_back(normals[face->GetNormalIndex(j)].z);
				} else {
					temp_normals.push_back(0.0f);
					temp_normals.push_back(0.0f);
					temp_normals.push_back(0.0f);
				}
			}
		}

		indexes = new unsigned int[temp_vertices.size()];
		for(unsigned int i = 0; i < temp_vertices.size(); i++) 
			indexes[i] = i;

		VertexArrayObject * obj = new VertexArrayObject(3, bufferUsage);

		obj->setElementsData(sizeof(unsigned int), temp_vertices.size() / 3, indexes);

		obj->setBufferData(0, temp_vertices.size() * sizeof(float), &temp_vertices[0]);
		obj->setBufferData(1, temp_normals.size() * sizeof(float), &temp_normals[0]);
		obj->setBufferData(2, temp_texCoords.size() * sizeof(float), &temp_texCoords[0]);

		obj->setVertexAttribute(0, 0, 3, GL_FLOAT);
		obj->setVertexAttribute(1, 1, 3, GL_FLOAT);
		obj->setVertexAttribute(2, 2, 2, GL_FLOAT);

		return obj;
	}

	vector<vec3> * ObjectFile::GetVertices() {
		vector<vec3> * res = new vector<vec3>();

		for(int i = 0; i < this->vertices.size(); i++)
			res->push_back(this->vertices[i]);

		return res;
	}

	vector<vec3> * ObjectFile::GetNormals() {
		vector<vec3> * res = new vector<vec3>();

		for(int i = 0; i < this->normals.size(); i++)
			res->push_back(this->normals[i]);

		return res;
	}

	vector<vec2> * ObjectFile::GetTexCoords() {
		vector<vec2> * res = new vector<vec2>();

		for(int i = 0; i < this->texCoords.size(); i++)
			res->push_back(this->texCoords[i]);

		return res;
	}

	vector<Face> * ObjectFile::GetFaces() {
		vector<Face> * res = new vector<Face>;

		for(int i = 0; i < this->faces.size(); i++)
			res->push_back(*(this->faces[i]));

		return res;
	}

	vector<Face> * ObjectFile::GetFaces(string material) {
		vector<Face> * res = new vector<Face>;

		for(int i = 0; i < this->faces.size(); i++) {
			if(material.compare(faces[i]->getMaterial()) == 0)
				res->push_back(*(this->faces[i]));
		}

		return res;
	}

	ObjectFile * ObjectFile::load(const string &fileName) {
		ObjectFile *obj = new ObjectFile();
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
				Face *face = new Face();
				while(!ss.eof()) {

					ss >> temp;
					stringstream ss2(temp);

					int v = 0, n = 0, t = 0;
					int ind1 = temp.find("/", 0);
					int ind2 = temp.find("/", ind1 + 1);

					ss2 >> v; // Vertex

					if(ind1 > 0)
						if(ind2 < 0)
							ss2 >> ch >> t; // Texture
						else
							ss2 >> ch >> t >> ch >> n; // Normal and texture

					face->addVertex(v-1, t-1, n-1);
					face->setGroup(currentGroup);
					face->setMaterial(currentMaterial);
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