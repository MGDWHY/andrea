#pragma once

#include <gl\glew.h>
#include "vecmath.h"

namespace glutils { 

	struct material {
	public:
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 emissive;
		float shininess;
		material() { 
			ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			emissive = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			shininess = 128.0f; 
		}
	};

	struct light {
	public:
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 position;
		vec3 spotDirection;
		float spotCutoff;
		light() {
			ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			specular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			spotDirection = vec3(0.0f, -1.0f, 0.0f);
			spotCutoff = 180.0f; 
		}
	};
}