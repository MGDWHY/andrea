#pragma once

#include <cmath>
#include <gl\glew.h>
#include <gl\glut.h>


namespace glutils {

	union vec2 {
		struct { GLfloat x, y;  };
		struct { GLfloat r, g; };
		GLfloat xy[2];
		GLfloat rg[2];
		vec2();
		vec2(GLfloat x, GLfloat y);
	};
	union vec3 {
		struct { GLfloat x, y, z;  };
		struct { GLfloat r, g, b; };
		GLfloat xyz[3];
		GLfloat rgb[3];
		vec3();
		vec3(GLfloat x, GLfloat y, GLfloat z);
	};
	union vec4 {
	public:
		struct { GLfloat x, y, z, w; };
		struct { GLfloat r, g, b, a; };
		GLfloat xyzw[4];
		GLfloat rgba[4];
		vec4();
		vec4(const vec3 &v, GLfloat w);
		vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	};

	union ivec2 {
		struct { GLint x, y;  };
		struct { GLint r, g; };
		GLint xy[2];
		GLint rg[2];
		ivec2();
		ivec2(GLint x, GLint y);
	};
	union ivec3 {
		struct { GLint x, y, z;  };
		struct { GLint r, g, b; };
		GLint xyz[3];
		GLint rgb[3];
		ivec3();
		ivec3(GLint x, GLint y, GLint z);
	};
	union ivec4 {
	public:
		struct { GLint x, y, z, w; };
		struct { GLint r, g, b, a; };
		GLfloat xyzw[4];
		GLfloat rgba[4];
		ivec4();
		ivec4(const ivec3 &v, GLint w);
		ivec4(GLint x, GLint y, GLint z, GLint w);
	};

	union mat3 {
	public:
		GLfloat elements[9];
		struct {
			vec3 rows[3];
		};
		mat3();
		mat3(GLfloat *elements);

		void toIdentity();
	};
	union mat4 {
	public:
		GLfloat elements[16];
		struct {
			vec4 rows[4];
		};

		mat4();
		mat4(GLfloat *elements);
		
		void toIdentity();
	};


	//-----------------------VECTORS--------------------
	vec2 scale(const vec2 &v, GLfloat t);
	vec3 scale(const vec3 &v, GLfloat t);

	vec2 add(const vec2 &v1, const vec2 &v2);
	vec3 add(const vec3 &v1, const vec3 &v2);

	vec2 lerp(const vec2 &v1, const vec2 &v2, GLfloat t);
	vec3 lerp(const vec3 &v1, const vec3 &v2, GLfloat t);

	vec2 sub(const vec2 &v1, const vec2 &v2);
	vec3 sub(const vec3 &v1, const vec3 &v2);

	vec2 normalize(const vec2 &v);
	vec3 normalize(const vec3 &v);

	vec3 cross(const vec3 &v1, const vec3 &v2);

	GLfloat dot(const vec3 &v1, const vec3 &v2);

	GLfloat length(const vec2 &v);
	GLfloat length(const vec3 &v);

	//------------------------MATRICES---------------------------

	mat3 mult(const mat3 &m1, const mat3 &m2);
	mat4 mult(const mat4 &m1, const mat4 &m2);

	mat3 transpose(const mat3 &m);
	mat4 transpose(const mat4 &m);
}