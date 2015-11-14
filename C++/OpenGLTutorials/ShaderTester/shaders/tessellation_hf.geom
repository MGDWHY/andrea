#version 330

#define LOW_LEVEL 2
#define MID_LEVEL 4
#define MAX_LEVEL 8

uniform mat4 in_ProjectionMatrix;

uniform float in_HFMaxHeight;
uniform float in_FrustumDepth; 

uniform sampler2D in_HF;

uniform vec3 in_HFNormal;

layout(triangles) in;
layout(triangle_strip, max_vertices = 128) out;

in vec3 normal[3];
in vec2 texCoord[3];
in vec2 hfCoord[3];
int subdivisionLevel[3];

vec3 p0, p1, p2;
vec2 h0, h1, h2;
vec2 t0, t1, t2;

out vec2 fgTexCoord;

vec4 getHFDisplacement(float i, float k) {
	float hfValue = texture(in_HF, h0 + h1 * i + h2 * k).r * in_HFMaxHeight;
	return vec4(in_HFNormal, 0.0) * hfValue;
}

vec4 getNewVertex(float i, float k) {
	return vec4(p0 + p1 * i + p2 * k, 1.0f);
}

vec2 getTexCoord(float i, float k) {
	return vec2(t0 + t1 *i + t2 * k);
}

void main() {
	
	vec3 tCentroid = ( gl_in[0].gl_Position.xyz +
						gl_in[1].gl_Position.xyz +
						gl_in[2].gl_Position.xyz ) / 3.0f;
	
	float depth = - tCentroid.z / in_FrustumDepth;
	

	
	int level;

	#ifdef ADAPTIVE
		float depth = - tCentroid.z / inFrustumDepth;
		if(depth < 0.4)
			level = MAX_LEVEL;
		else if(depth < 0.6)
			level = MID_LEVEL;
		else if(depth < 0.7)
			level = LOW_LEVEL;
		else
			level = 1;
	#else
		level = MAX_LEVEL;
	#endif
	
	p0 = gl_in[0].gl_Position.xyz;
	p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

	h0 = hfCoord[0];
	h1 = hfCoord[1] - h0;
	h2 = hfCoord[2] - h0;

	t0 = texCoord[0];
	t1 = texCoord[1] - t0;
	t2 = texCoord[2] - t0;

	float ztep = 1.0f / level;

	for(float i = 0.0; i < 1.0f; i += ztep) {
		float k = 0.0;
	
		for(k = 0.0; k < 1.0f - i; k += ztep) {
			
			fgTexCoord = getTexCoord(i, k);
			gl_Position = in_ProjectionMatrix * (getNewVertex(i, k) + getHFDisplacement(i,k));
			EmitVertex();

			fgTexCoord = getTexCoord(i + ztep, k);
			gl_Position = in_ProjectionMatrix * (getNewVertex(i + ztep, k) + getHFDisplacement(i + ztep, k));
			EmitVertex();
		}

		fgTexCoord = getTexCoord(i, k);
		gl_Position = in_ProjectionMatrix * (getNewVertex(i, k) + getHFDisplacement(i,k));
		EmitVertex();
		EndPrimitive();
	}
	
}