#pragma once

#include <cmath>

namespace Vecmath {

	union vec2;
	union vec3;
	union vec4;
	
	union ivec2;
	union ivec3;
	union ivec4;

	union vec2 {
		struct { float x, y;  };
		struct { float r, g; };
		float xy[2];
		float rg[2];
		vec2();
		vec2(const ivec2 &v);
		vec2(float * elements);
		vec2(float x, float y);
	};
	union vec3 {
		struct { float x, y, z;  };
		struct { float r, g, b; };
		float xyz[3];
		float rgb[3];
		vec3();
		vec3(float * elements);
		vec3(float x, float y, float z);
	};
	union vec4 {
	public:
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float xyzw[4];
		float rgba[4];
		vec4();
		vec4(const vec3 &v, float w);
		vec4(float x, float y, float z, float w);
	};

	union ivec2 {
		struct { int x, y;  };
		struct { int r, g; };
		int xy[2];
		int rg[2];
		ivec2();
		ivec2(int x, int y);
	};
	union ivec3 {
		struct { int x, y, z;  };
		struct { int r, g, b; };
		int xyz[3];
		int rgb[3];
		ivec3();
		ivec3(int x, int y, int z);
	};
	union ivec4 {
	public:
		struct { int x, y, z, w; };
		struct { int r, g, b, a; };
		float xyzw[4];
		float rgba[4];
		ivec4();
		ivec4(const ivec3 &v, int w);
		ivec4(int x, int y, int z, int w);
	};

	union mat3 {
	public:
		float elements[9];
		struct {
			vec3 rows[3];
		};
		mat3();
		mat3(float *elements);

		void toIdentity();
	};
	union mat4 {
	public:
		float elements[16];
		struct {
			vec4 rows[4];
		};

		mat4();
		mat4(float *elements);
		
		void toIdentity();
		void toRotation(float theta, float x, float y, float z);

		static mat4 Identity();
	};


	//-----------------------VECTORS--------------------

	ivec2 scale(const ivec2 &v, int x);

	vec2 scale(const ivec2 &v, float t);
	vec2 scale(const vec2 &v, float t);
	vec3 scale(const vec3 &v, float t);
	vec4 scale(const vec4 &v, float t);

	ivec2 add(const ivec2 &v1, const ivec2 &v2);

	vec2 add(const vec2 &v1, const vec2 &v2);
	vec3 add(const vec3 &v1, const vec3 &v2);
	vec4 add(const vec4 &v1, const vec4 &v2);

	vec2 lerp(const vec2 &v1, const vec2 &v2, float t);
	vec3 lerp(const vec3 &v1, const vec3 &v2, float t);


	ivec2 sub(const ivec2 &v1, const ivec2 &v2);

	vec2 sub(const vec2 &v1, const vec2 &v2);
	vec3 sub(const vec3 &v1, const vec3 &v2);

	vec2 normalize(const vec2 &v);
	vec3 normalize(const vec3 &v);

	vec3 cross(const vec3 &v1, const vec3 &v2);	

	vec3 project(const vec3& p, const vec3& o, const vec3& n);

	float dot(const vec3 &v1, const vec3 &v2);
	float dot(const vec4 &v1, const vec4 &v2);

	float length(const vec2 &v);
	float length(const vec3 &v);

	bool operator==(const ivec2 &v1, const ivec2 &v2);

	bool operator==(const vec2 &v1, const vec2 &v2);
	bool operator==(const vec3 &v1, const vec3 &v2);
	bool operator==(const vec4 &v1, const vec4 &v2);

	ivec2 operator+(const ivec2 &v1, const ivec2 &v2);

	vec2 operator+(const vec2 &v1, const vec2 &v2);
	vec3 operator+(const vec3 &v1, const vec3 &v2);
	vec4 operator+(const vec4 &v1, const vec4 &v2);

	ivec2 operator-(const ivec2 &v1, const ivec2 &v2);

	vec2 operator-(const vec2 &v1, const vec2 &v2);
	vec3 operator-(const vec3 &v1, const vec3 &v2);
	vec4 operator-(const vec4 &v1, const vec4 &v2);

	ivec2 operator*(const ivec2 &v1, int x);
	
	vec2 operator*(const ivec2 &v1, float t);
	vec2 operator*(const vec2 &v1, float t);
	vec3 operator*(const vec3 &v1, float t);
	vec4 operator*(const vec4 &v1, float t);




	//------------------------MATRICES---------------------------

	mat3 mult(const mat3 &m1, const mat3 &m2);
	mat4 mult(const mat4 &m1, const mat4 &m2);

	vec4 multvec(const mat4 &m, const vec4 &v);
	vec3 multvec(const mat3 &m, const vec3 &v);

	mat3 transpose(const mat3 &m);
	mat4 transpose(const mat4 &m);

	mat4 invert(const mat4 &m);

	float determinant(const mat3 &m);

	mat3 operator*(const mat3 &m1, const mat3 &m2);
	mat4 operator*(const mat4 &m1, const mat4 &m2);

	vec3 operator*(const mat3 &m1, const vec3 &v1);
	vec4 operator*(const mat4 &m1, const vec4 &v1);


}