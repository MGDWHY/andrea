#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "WFMaterial.h"

using namespace std;

namespace WaveFront {

	class WFMaterial;
	
	class WFMaterialLibrary {
	private:
		// File tokens
		static const string T_NEWMTL;
		static const string T_KA;
		static const string T_KD;
		static const string T_KS;
		static const string T_MAPKD;
		static const string T_COMMENT;

		vector<WFMaterial> materials;

		WFMaterialLibrary();

	public:

		static WFMaterialLibrary* FromFile(const string &fileName);

		~WFMaterialLibrary();

		int GetMaterialCount();

		WFMaterial GetMaterial(const int index);
	};

}
