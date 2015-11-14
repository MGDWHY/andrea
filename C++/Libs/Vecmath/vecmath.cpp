#include "vecmath.h"

namespace Vecmath {

	//---------------------------VECTORS--------------------------------


	// functions

	vec2::vec2() { x = y = 0.0f; }
	vec2::vec2(const ivec2 &v) { this->x = v.x; this->y = v.y; }
	vec2::vec2(float * elements) {
		for(int i = 0; i < 3; i++)
			this->xy[i] = elements[i];
	}
	vec2::vec2(float x, float y) {this->x = x; this->y = y; }

	vec3::vec3() { x = y = z = 0.0f; }
	vec3::vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	vec3::vec3(float * elements) { 
		for(int i = 0; i < 3; i++)
			this->xyz[i] = elements[i];
	}
	vec4::vec4() { x = y = z = w = 0.0f; }
	vec4::vec4(const vec3 &v, float w) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;}
	vec4::vec4(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w;}

	ivec2::ivec2() { x = y = 0; }
	ivec2::ivec2(int x, int y) {this->x = x; this->y = y; }

	ivec3::ivec3() { x = y = z = 0; }
	ivec3::ivec3(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }

	ivec4::ivec4() { x = y = z = w = 0; }
	ivec4::ivec4(const ivec3 &v, int w) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = w;}
	ivec4::ivec4(int x, int y, int z, int w) { this->x = x; this->y = y; this->z = z; this->w = w;}

	ivec2 scale(const ivec2 &v, int x) { return ivec2(v.x*x, v.y*x); }

	vec2 scale(const ivec2 &v, float t) { return vec2(v.x*t, v.y*t);}

	vec2 scale(const vec2 &v, float t) { return vec2(v.x*t, v.y*t);}
	vec3 scale(const vec3 &v, float t) { return vec3(v.x*t, v.y*t, v.z*t); }
	vec4 scale(const vec4 &v, float t) { return vec4(v.x*t, v.y*t, v.z*t, v.w*t); }

	vec2 lerp(const vec2 &v1, const vec2 &v2, float t) { return add(scale(v1, 1-t), scale(v2, t));}
	vec3 lerp(const vec3 &v1, const vec3 &v2, float t) { return add(scale(v1, 1-t), scale(v2, t));}

	ivec2 add(const ivec2 &v1, const ivec2 &v2) { return ivec2(v1.x + v2.x, v1.y + v2.y); }

	vec2 add(const vec2 &v1, const vec2 &v2) { return vec2(v1.x + v2.x, v1.y + v2.y);}
	vec3 add(const vec3 &v1, const vec3 &v2) { return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	vec4 add(const vec4 &v1, const vec4 &v2) { return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w); }

	ivec2 sub(const ivec2 &v1, const ivec2 &v2) { return ivec2(v1.x - v2.x, v1.y - v2.y); }

	vec2 sub(const vec2 &v1, const vec2 &v2) { return vec2(v1.x - v2.x, v1.y - v2.y);}
	vec3 sub(const vec3 &v1, const vec3 &v2) { return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	vec4 sub(const vec4 &v1, const vec4 &v2) { return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w); }

	vec3 project(const vec3& p, const vec3& o, const vec3& n) {
		return p - n * dot((p - o), n);
	}

	vec2 normalize(const vec2 &v) { 
		float l = length(v);
		return vec2(v.x/l, v.y/l);
	}

	vec3 normalize(const vec3 &v) {
		float l = length(v);
		return vec3(v.x/l, v.y/l, v.z/l);
	}

	vec3 cross(const vec3 &v1, const vec3 &v2) {
		float x, y, z;
		x = v1.y * v2.z - v2.y * v1.z;
		y = v1.z * v2.x - v2.z * v1.x;
		z = v1.x * v2.y - v2.x * v1.y;
		return vec3(x, y, z);
	}

	float dot(const vec3 &v1, const vec3 &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
	float dot(const vec4 &v1, const vec4 &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

	float length(const vec2 &v) { return sqrt(pow(v.x, 2) + pow(v.y, 2));}
	float length(const vec3 &v) { return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));}

	// operators

	bool operator==(const ivec2 &v1, const ivec2 &v2) { return v1.x == v2.x && v1.y == v2.y; }

	bool operator==(const vec2 &v1, const vec2 &v2) { return v1.x == v2.x && v1.y == v2.y; }
	bool operator==(const vec3 &v1, const vec3 &v2) { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z; }
	bool operator==(const vec4 &v1, const vec4 &v2)  { return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w; }

	ivec2 operator+(const ivec2 &v1, const ivec2 &v2) { return add(v1, v2); }

	vec2 operator+(const vec2 &v1, const vec2 &v2) { return add(v1, v2); }
	vec3 operator+(const vec3 &v1, const vec3 &v2) { return add(v1, v2); }
	vec4 operator+(const vec4 &v1, const vec4 &v2) { return add(v1, v2); }

	ivec2 operator-(const ivec2 &v1, const ivec2 &v2) { return sub(v1, v2); }

	vec2 operator-(const vec2 &v1, const vec2 &v2) { return sub(v1, v2); }
	vec3 operator-(const vec3 &v1, const vec3 &v2) { return sub(v1, v2); }
	vec4 operator-(const vec4 &v1, const vec4 &v2) { return sub(v1, v2); }

	ivec2 operator*(const ivec2 &v1, int x) { return scale(v1, x); }

	vec2 operator*(const ivec2 &v1, float t) { return scale(v1, t); }
	vec2 operator*(const vec2 &v1, float t) { return scale(v1, t); }
	vec3 operator*(const vec3 &v1, float t) { return scale(v1, t); }
	vec4 operator*(const vec4 &v1, float t) { return scale(v1, t); }

	//----------------------------MATRICES------------------------------
	mat3::mat3() {
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				elements[i*3 +j] = 0.0f;
	}

	mat3::mat3(float *elements) {
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

	mat4::mat4(float *elements) {
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

	void mat4::toRotation(float theta, float x, float y, float z) {
		float ct = cos(theta);
		float st = sin(theta);

		this->elements[0] = ct + x * x * (1.0f - ct);
		this->elements[1] = x * y * (1.0f - ct) - z * st;
		this->elements[2] = x * z * (1.0f - ct) + y * st;
		this->elements[3] = 0.0f;

		this->elements[4] = y * x * (1.0f - ct) + z * st;
		this->elements[5] = ct + y * y * (1.0f - ct);
		this->elements[6] = y * z * (1.0f - ct) - x * st;
		this->elements[7] = 0.0f;

		this->elements[8] = z * x * (1.0f - ct) - y * st;
		this->elements[9] = z * y * (1.0f - ct) + x * st;
		this->elements[10] = ct + z * z * (1.0f - ct);
		this->elements[11] = 0.0f;

		this->elements[12] = 0.0f;
		this->elements[13] = 0.0f;
		this->elements[14] = 0.0f;
		this->elements[15] = 1.0f;
	}

	mat4 mat4::Identity() {
		mat4 result;
		result.toIdentity();
		return result;
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

	vec4 multvec(const mat4 &m1, const vec4 &v) {		
		return vec4(
			dot(m1.rows[0], v),
			dot(m1.rows[1], v),
			dot(m1.rows[2], v),
			dot(m1.rows[3], v)
		);
	}

	vec3 multvec(const mat3 &m1, const vec3 &v) {		
		return vec3(
			dot(m1.rows[0], v),
			dot(m1.rows[1], v),
			dot(m1.rows[2], v)
		);
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

	mat4 invert(const mat4 &m) {

		float inv[16], det;
		mat4 result;

		int i;

		inv[0] =   m.elements[5]*m.elements[10]*m.elements[15] - m.elements[5]*m.elements[11]*m.elements[14] - m.elements[9]*m.elements[6]*m.elements[15]
			+ m.elements[9]*m.elements[7]*m.elements[14] + m.elements[13]*m.elements[6]*m.elements[11] - m.elements[13]*m.elements[7]*m.elements[10];
		inv[4] =  -m.elements[4]*m.elements[10]*m.elements[15] + m.elements[4]*m.elements[11]*m.elements[14] + m.elements[8]*m.elements[6]*m.elements[15]
			- m.elements[8]*m.elements[7]*m.elements[14] - m.elements[12]*m.elements[6]*m.elements[11] + m.elements[12]*m.elements[7]*m.elements[10];
		inv[8] =   m.elements[4]*m.elements[9]*m.elements[15] - m.elements[4]*m.elements[11]*m.elements[13] - m.elements[8]*m.elements[5]*m.elements[15]
			+ m.elements[8]*m.elements[7]*m.elements[13] + m.elements[12]*m.elements[5]*m.elements[11] - m.elements[12]*m.elements[7]*m.elements[9];
		inv[12] = -m.elements[4]*m.elements[9]*m.elements[14] + m.elements[4]*m.elements[10]*m.elements[13] + m.elements[8]*m.elements[5]*m.elements[14]
			- m.elements[8]*m.elements[6]*m.elements[13] - m.elements[12]*m.elements[5]*m.elements[10] + m.elements[12]*m.elements[6]*m.elements[9];
		inv[1] =  -m.elements[1]*m.elements[10]*m.elements[15] + m.elements[1]*m.elements[11]*m.elements[14] + m.elements[9]*m.elements[2]*m.elements[15]
			- m.elements[9]*m.elements[3]*m.elements[14] - m.elements[13]*m.elements[2]*m.elements[11] + m.elements[13]*m.elements[3]*m.elements[10];
		inv[5] =   m.elements[0]*m.elements[10]*m.elements[15] - m.elements[0]*m.elements[11]*m.elements[14] - m.elements[8]*m.elements[2]*m.elements[15]
			+ m.elements[8]*m.elements[3]*m.elements[14] + m.elements[12]*m.elements[2]*m.elements[11] - m.elements[12]*m.elements[3]*m.elements[10];
		inv[9] =  -m.elements[0]*m.elements[9]*m.elements[15] + m.elements[0]*m.elements[11]*m.elements[13] + m.elements[8]*m.elements[1]*m.elements[15]
			- m.elements[8]*m.elements[3]*m.elements[13] - m.elements[12]*m.elements[1]*m.elements[11] + m.elements[12]*m.elements[3]*m.elements[9];
		inv[13] =  m.elements[0]*m.elements[9]*m.elements[14] - m.elements[0]*m.elements[10]*m.elements[13] - m.elements[8]*m.elements[1]*m.elements[14]
			+ m.elements[8]*m.elements[2]*m.elements[13] + m.elements[12]*m.elements[1]*m.elements[10] - m.elements[12]*m.elements[2]*m.elements[9];
		inv[2] =   m.elements[1]*m.elements[6]*m.elements[15] - m.elements[1]*m.elements[7]*m.elements[14] - m.elements[5]*m.elements[2]*m.elements[15]
			+ m.elements[5]*m.elements[3]*m.elements[14] + m.elements[13]*m.elements[2]*m.elements[7] - m.elements[13]*m.elements[3]*m.elements[6];
		inv[6] =  -m.elements[0]*m.elements[6]*m.elements[15] + m.elements[0]*m.elements[7]*m.elements[14] + m.elements[4]*m.elements[2]*m.elements[15]
			- m.elements[4]*m.elements[3]*m.elements[14] - m.elements[12]*m.elements[2]*m.elements[7] + m.elements[12]*m.elements[3]*m.elements[6];
		inv[10] =  m.elements[0]*m.elements[5]*m.elements[15] - m.elements[0]*m.elements[7]*m.elements[13] - m.elements[4]*m.elements[1]*m.elements[15]
			+ m.elements[4]*m.elements[3]*m.elements[13] + m.elements[12]*m.elements[1]*m.elements[7] - m.elements[12]*m.elements[3]*m.elements[5];
		inv[14] = -m.elements[0]*m.elements[5]*m.elements[14] + m.elements[0]*m.elements[6]*m.elements[13] + m.elements[4]*m.elements[1]*m.elements[14]
			- m.elements[4]*m.elements[2]*m.elements[13] - m.elements[12]*m.elements[1]*m.elements[6] + m.elements[12]*m.elements[2]*m.elements[5];
		inv[3] =  -m.elements[1]*m.elements[6]*m.elements[11] + m.elements[1]*m.elements[7]*m.elements[10] + m.elements[5]*m.elements[2]*m.elements[11]
			- m.elements[5]*m.elements[3]*m.elements[10] - m.elements[9]*m.elements[2]*m.elements[7] + m.elements[9]*m.elements[3]*m.elements[6];
		inv[7] =   m.elements[0]*m.elements[6]*m.elements[11] - m.elements[0]*m.elements[7]*m.elements[10] - m.elements[4]*m.elements[2]*m.elements[11]
			+ m.elements[4]*m.elements[3]*m.elements[10] + m.elements[8]*m.elements[2]*m.elements[7] - m.elements[8]*m.elements[3]*m.elements[6];
		inv[11] = -m.elements[0]*m.elements[5]*m.elements[11] + m.elements[0]*m.elements[7]*m.elements[9] + m.elements[4]*m.elements[1]*m.elements[11]
			- m.elements[4]*m.elements[3]*m.elements[9] - m.elements[8]*m.elements[1]*m.elements[7] + m.elements[8]*m.elements[3]*m.elements[5];
		inv[15] =  m.elements[0]*m.elements[5]*m.elements[10] - m.elements[0]*m.elements[6]*m.elements[9] - m.elements[4]*m.elements[1]*m.elements[10]
			+ m.elements[4]*m.elements[2]*m.elements[9] + m.elements[8]*m.elements[1]*m.elements[6] - m.elements[8]*m.elements[2]*m.elements[5];

		det = m.elements[0]*inv[0] + m.elements[1]*inv[4] + m.elements[2]*inv[8] + m.elements[3]*inv[12];

		if (det == 0)
				return false;

		det = 1.0f / det;

		for (i = 0; i < 16; i++)
			result.elements[i] = inv[i] * det;

		return result;

	}

	float determinant(const mat3 &m) {
		return m.elements[0] * m.elements[4] * m.elements[8] +
			m.elements[1] * m.elements[5] * m.elements[6] +
			m.elements[2] * m.elements[3] * m.elements[7] -
			(
				m.elements[6] * m.elements[4] * m.elements[2] +
				m.elements[7] * m.elements[5] * m.elements[0] +
				m.elements[8] * m.elements[3] * m.elements[1]
			);
	}

	// operators

	mat3 operator*(const mat3 &m1, const mat3 &m2) { return mult(m1, m2); }
	mat4 operator*(const mat4 &m1, const mat4 &m2)  { return mult(m1, m2); }

	vec3 operator*(const mat3 &m1, const vec3 &v1) { return multvec(m1, v1); }
	vec4 operator*(const mat4 &m1, const vec4 &v1) { return multvec(m1, v1); }
}