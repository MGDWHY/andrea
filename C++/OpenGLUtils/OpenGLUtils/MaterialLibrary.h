#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "Material.h"
#include "Logger.h"

using namespace std;

namespace glutils {
	
	class MaterialLibrary {
	private:
		// File tokens
		static const string T_NEWMTL;
		static const string T_KA;
		static const string T_KD;
		static const string T_KS;
		static const string T_MAPKD;
		static const string T_COMMENT;

		vector<Material> materials;

		MaterialLibrary();

	public:

		static MaterialLibrary* load(const string &fileName);

		~MaterialLibrary();

		int getMaterialCount();

		Material getMaterial(const int index);
	};

}
