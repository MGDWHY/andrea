#include "vecmath.h"

namespace glutils {

	//---------------------------VECTORS--------------------------------

	vec2::vec2() { x = y = 0.0f; }
	vec2::vec2(GLfloat x, GLfloat y) {this->x = x; this->y = y; }

	vec3::vec3() { x = y = z = 0.0f; }
	vec3::vec3(GLfloat x, GLfloat y, GLfloat z) { this->x = x; this->y = y; this->z = z; }

	vec4::vec4() { x = y = z = w = 0.0f; }
	vec4::vec4(const vec3 &v, GLfloat w) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;}
	vec4::vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) { this->x = x; this->y = y; this->z = z; this->w = w;}

	ivec2::ivec2() { x = y = 0; }
	ivec2::ivec2(GLint x, GLint y) {this->x = x; this->y = y; }

	ivec3::ivec3() { x = y = z = 0; }
	ivec3::ivec3(GLint x, GLint y, GLint z) { this->x = x; this->y = y; this->z = z; }

	ivec4::ivec4() { x = y = z = w = 0; }
	ivec4::ivec4(const ivec3 &v, GLint w) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;}
	ivec4::ivec4(GLint x, GLint y, GLint z, GLint w) { this->x = x; this->y = y; this->z = z; this->w = w;}

	vec2 scale(const vec2 &v, GLfloat t) { return vec2(v.x*t, v.y*t);}
	vec3 scale(const vec3 &v, GLfloat t) { return vec3(v.x*t, v.y*t, v.z*t); }

	vec2 lerp(const vec2 &v1, const vec2 &v2, GLfloat t) { return add(scale(v1, 1-t), scale(v2, t));}
	vec3 lerp(const vec3 &v1, const vec3 &v2, GLfloat t) { return add(scale(v1, 1-t), scale(v2, t));}

	vec2 add(const vec2 &v1, const vec2 &v2) { return vec2(v1.x + v2.x, v1.y + v2.y);}
	vec3 add(const vec3 &v1, const vec3 &v2) { return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

	vec2 sub(const vec2 &v1, const vec2 &v2) { return vec2(v1.x - v2.x, v1.y - v2.y);}
	vec3 sub(const vec3 &v1, const vec3 &v2) { return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }

	vec2 normalize(const vec2 &v) { 
		GLfloat l = length(v);
		return vec2(v.x/l, v.y/l);
	}

	vec3 normalize(const vec3 &v) {
		GLfloat l = length(v);
		return vec3(v.x/l, v.y/l, v.z/l);
	}

	vec3 cross(const vec3 &v1, const vec3 &v2) {
		GLfloat x, y, z;
		x = v1.y * v2.z - v2.y * v1.z;
		y = v1.z * v2.x - v2.z * v1.x;
		z = v1.x * v2.y - v2.x * v1.y;
		return vec3(x, y, z);
	}

	GLfloat dot(const vec3 &v1, const vec3 &v2) {
		return v1.x * v2.x + v1.y * v2.y + v2.z * v2.z;
	}

	GLfloat length(const vec2 &v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	GLfloat length(const vec3 &v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	}

	//----------------------------MATRICES------------------------------
	mat3::mat3() {
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				elements[i*3 +j] = 0.0f;
	}

	mat3::mat3(GLfloat *elements) {
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				this->elements[i*3 +j] = *(elements + i*3 + j);
	}

	void mat3::toIdentity() {
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				if( j == i )
					elements[i*3 + j] = 1.0f;
				else
					elements[i*3 + j] = 0.0f;
	}

	mat4::mat4() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				elements[i*4 +j] = 0.0f;
	}

	mat4::mat4(GLfloat *elements) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				this->elements[i*4 +j] = *(elements + i*4 + j);
	}

	void mat4::toIdentity() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(j == i )
					elements[i*4 + j] = 1.0f;
				else
					elements[i*4 + j] = 0.0f;
	}


	mat3 mult(const mat3 &m1, const mat3 &m2) {
		mat3 result;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++) {
				result.elements[i*3 + j] = 0.0f;
				for(int e = 0; e < 3; e++)
					result.elements[i*3 + j] += m1.elements[i*3 + e] * m2.elements[j + e*3];
			}

		return result;
	}

	mat4 mult(const mat4 &m1, const mat4 &m2) {
		mat4 result;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++) {
				result.elements[i*4 + j] = 0.0f;
				for(int e = 0; e < 4; e++)
					result.elements[i*4 + j] += m1.elements[i*4 + e] * m2.elements[j + e*4];
			}

		return result;
	}

	mat3 transpose(const mat3 &m) {
		mat3 result;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				result.elements[i*3 + j] = m.elements[j*3 + i];
		return result;
	}

	mat4 transpose(const mat4 &m) {
		mat4 result;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				result.elements[i*4 + j] = m.elements[j*4 + i];
		return result;
	}
}