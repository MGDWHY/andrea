#pragma once

#include <string>
#include "vecmath.h"

using namespace Vecmath;
using namespace std;

namespace glutils {
	
	class Material {
	private:
		
		string name, mapKd;
		vec3 Ka, Kd, Ks;

		void setName(string name);
		void setKa(vec3 Ka);
		void setKd(vec3 Kd);
		void setKs(vec3 Ks);
		void setMapKd(string mapKd);

		Material();
	public:
		
		~Material();
		
		string getName();
		vec3 getKa();
		vec3 getKd();
		vec3 getKs();
		string getMapKd();

		friend class MaterialLibrary;
	};

}
