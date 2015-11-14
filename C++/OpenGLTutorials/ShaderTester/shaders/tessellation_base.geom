#version 330

#define LOW_LEVEL 2
#define MID_LEVEL 5
#define MAX_LEVEL 10

uniform mat4 in_ProjectionMatrix;

uniform float in_FrustumDepth;

layout(triangles) in;
layout(triangle_strip, max_vertices = 180) out;

in vec3 normal[3];
in vec2 texCoord[3];

void main() {
	
	vec3 p0, p1, p2;
	
	vec3 tCentroid = ( gl_in[0].gl_Position.xyz +
						gl_in[1].gl_Position.xyz +
						gl_in[2].gl_Position.xyz ) / 3.0f;
	
	float depth = - tCentroid.z / in_FrustumDepth;
	
	int level;


	if(depth < 0.4)
		level = MAX_LEVEL;
	else if(depth < 0.6)
		level = MID_LEVEL;
	else if(depth < 0.7)
		level = LOW_LEVEL;
	else
		level = 1;

	p0 = gl_in[0].gl_Position.xyz;
	p1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	p2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	
	float ztep = 1.0f / level;

	for(float i = 0.0; i < 1.0f; i += ztep) {
		float k = 0.0;
		for(k = 0.0; k < 1.0f - i; k += ztep) {
			gl_Position = in_ProjectionMatrix * vec4(p0 + p1 * i + p2 * k, 1.0f);
			EmitVertex();
			gl_Position = in_ProjectionMatrix * vec4(p0 + p1 * (i + ztep) + p2 * k, 1.0f);
			EmitVertex();
		}
		gl_Position = in_ProjectionMatrix * vec4(p0 + p1 * i + p2 * k, 1.0f);
		EmitVertex();
		EndPrimitive();
	}
	
/*	for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = in_ProjectionMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();*/
	
	
}