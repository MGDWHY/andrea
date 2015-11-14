#include "WFMaterial.h"

namespace WaveFront {

	WFMaterial::WFMaterial() {}
	WFMaterial::~WFMaterial() {}

	void WFMaterial::SetName(string name) { this->name = name;}
	void WFMaterial::SetKa(vec3 Ka) { this->Ka = Ka; }
	void WFMaterial::SetKd(vec3 Kd) { this->Kd = Kd; }
	void WFMaterial::SetKs(vec3 Ks) { this->Ks = Ks; }
	void WFMaterial::SetMapKd(string mapKd) { this->mapKd = mapKd; }

	string WFMaterial::GetName() { return this->name; }
	vec3 WFMaterial::GetKa() { return this->Ka; }
	vec3 WFMaterial::GetKd() { return this->Kd; }
	vec3 WFMaterial::GetKs() { return this->Ks; }
	string WFMaterial::GetMapKd() { return this->mapKd; }

	
}