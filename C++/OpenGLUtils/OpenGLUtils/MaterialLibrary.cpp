#include "MaterialLibrary.h"

namespace glutils {

	const string MaterialLibrary::T_NEWMTL = "newmtl";
	const string MaterialLibrary::T_KA = "Ka";
	const string MaterialLibrary::T_KD = "Kd";
	const string MaterialLibrary::T_KS = "Ks";
	const string MaterialLibrary::T_MAPKD = "map_Kd";
	const string MaterialLibrary::T_COMMENT = "#";

	MaterialLibrary::MaterialLibrary() {
		materials = vector<Material>();
	}

	MaterialLibrary::~MaterialLibrary() {
		materials.clear();
	}

	MaterialLibrary* MaterialLibrary::load(const string &fileName) {
		MaterialLibrary *matlib = new MaterialLibrary();
		ifstream file;
		int skippedLines = 0; // for debug
		file.open(fileName);

		if(!file.is_open()) {
			#ifdef GL_UTILS_LOG_ENABLED
				Logger::getDefaultLogger()->writeMessage(2, "MaterialLibrary::load()", "Can't open file: " + fileName);
			#endif
			return NULL;
		}

		#ifdef GL_UTILS_LOG_ENABLED
			Logger::getDefaultLogger()->writeMessage(0, "MaterialLibrary::load()", "------------------------------------");
			Logger::getDefaultLogger()->writeMessage(0, "MaterialLibrary::load()", "Loading file: " + fileName);
		#endif

		Material *current = NULL;

		while(file.good()) {
			
			float x, y, z;
			string line, op, temp;

			getline(file, line);
			stringstream ss(line);

			ss >> op;
			
			if(op.compare(T_NEWMTL) == 0) { // New material
				if(current != NULL)
					matlib->materials.push_back(*current); // adding the previous material if any

				ss >> temp;
				current = new Material();
				current->setName(temp);
			} else if(op.compare(T_KA) == 0) { // Ambient color
				ss >> x >> y >> z;
				current->setKa(vec3(x,y,z));
			} else if(op.compare(T_KD) == 0) { // Diffuse color
				ss >> x >> y >> z;
				current->setKd(vec3(x,y,z));
			} else if(op.compare(T_KS) == 0) { // Specular color
				ss >> x >> y >> z;
				current->setKs(vec3(x,y,z));
			} else if(op.compare(T_MAPKD) == 0) { // Texture (attenzione agli spazi nei percorsi dei file)
				ss >> temp;
				current->setMapKd(temp);
			} else if(op.compare(T_COMMENT) == 0) { // Comment
				#ifdef GL_UTILS_LOG_ENABLED
					Logger::getDefaultLogger()->writeMessage(0, "MaterialLibrary::load()", line);
				#endif			
			} else { // Unknown token
				#ifdef GL_UTILS_LOG_ENABLED
					skippedLines++;
				#endif		
			}
		}

		if(current != NULL)
			matlib->materials.push_back(*current); // adding the last material if any

		file.close();

		#ifdef GL_UTILS_LOG_ENABLED
			stringstream msgss;
			
			msgss	<< "------------------------------------" << endl
					<< "File loaded: " + fileName << endl
					<< "Skipped lines (unknown tokens): " << skippedLines << endl
					<< "------------------------------------" << endl
					<< "Total materials: " << matlib->materials.size();

			Logger::getDefaultLogger()->writeMessageStream(0, "MaterialLibrary::load()", msgss);

		#endif

		return matlib;
	}

	int MaterialLibrary::getMaterialCount() {
		return materials.size();
	}

	Material MaterialLibrary::getMaterial(const int index) {
		return materials[index];
	}

}