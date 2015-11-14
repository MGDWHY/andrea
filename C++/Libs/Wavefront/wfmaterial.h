#pragma once

#include <string>
#include "vecmath.h"
#include "WFMaterialLibrary.h"

using namespace Vecmath;
using namespace std;

namespace WaveFront {
	
	class WFMaterial {
	private:
		
		string name, mapKd;
		vec3 Ka, Kd, Ks;

		void SetName(string name);
		void SetKa(vec3 Ka);
		void SetKd(vec3 Kd);
		void SetKs(vec3 Ks);
		void SetMapKd(string mapKd);

		WFMaterial();
	public:
		
		~WFMaterial();
		
		string GetName();
		vec3 GetKa();
		vec3 GetKd();
		vec3 GetKs();
		string GetMapKd();

		friend class WFMaterialLibrary;
	};

}
