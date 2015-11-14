#include "Material.h"

namespace glutils {

	Material::Material() {}
	Material::~Material() {}

	void Material::setName(string name) { this->name = name;}
	void Material::setKa(vec3 Ka) { this->Ka = Ka; }
	void Material::setKd(vec3 Kd) { this->Kd = Kd; }
	void Material::setKs(vec3 Ks) { this->Ks = Ks; }
	void Material::setMapKd(string mapKd) { this->mapKd = mapKd; }

	string Material::getName() { return name; }
	vec3 Material::getKa() { return Ka; }
	vec3 Material::getKd() { return Kd; }
	vec3 Material::getKs() { return Ks; }
	string Material::getMapKd() { return mapKd; }

	
}