#include "WFMaterialLibrary.h"

namespace WaveFront {

	const string WFMaterialLibrary::T_NEWMTL = "newmtl";
	const string WFMaterialLibrary::T_KA = "Ka";
	const string WFMaterialLibrary::T_KD = "Kd";
	const string WFMaterialLibrary::T_KS = "Ks";
	const string WFMaterialLibrary::T_MAPKD = "map_Kd";
	const string WFMaterialLibrary::T_COMMENT = "#";

	WFMaterialLibrary::WFMaterialLibrary() {
		materials = vector<WFMaterial>();
	}

	WFMaterialLibrary::~WFMaterialLibrary() {
		materials.clear();
	}

	WFMaterialLibrary* WFMaterialLibrary::FromFile(const string &fileName) {
		WFMaterialLibrary *matlib = new WFMaterialLibrary();
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

		WFMaterial *current = NULL;

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
				current = new WFMaterial();
				current->SetName(temp);
			} else if(op.compare(T_KA) == 0) { // Ambient color
				ss >> x >> y >> z;
				current->SetKa(vec3(x,y,z));
			} else if(op.compare(T_KD) == 0) { // Diffuse color
				ss >> x >> y >> z;
				current->SetKd(vec3(x,y,z));
			} else if(op.compare(T_KS) == 0) { // Specular color
				ss >> x >> y >> z;
				current->SetKs(vec3(x,y,z));
			} else if(op.compare(T_MAPKD) == 0) { // Texture (attenzione agli spazi nei percorsi dei file)
				ss >> temp;
				current->SetMapKd(temp);
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

	int WFMaterialLibrary::GetMaterialCount() {
		return this->materials.size();
	}

	WFMaterial WFMaterialLibrary::GetMaterial(const int index) {
		return this->materials[index];
	}

}