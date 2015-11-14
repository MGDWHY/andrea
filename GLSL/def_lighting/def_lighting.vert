#version 330

uniform mat4 in_ProjectionMatrix;
uniform mat4 in_ModelViewMatrix;

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec3 in_Tangent;
layout(location = 3) in vec2 in_TexCoords;


out vec2 texCoords;
out mat3 TBNinverse;

void main() {
	gl_Position = in_ProjectionMatrix * in_ModelViewMatrix * in_Position;	
	texCoords = in_TexCoords;
	
	vec3 T = (in_ModelViewMatrix * vec4(in_Tangent, 0.0)).xyz;
	vec3 N = (in_ModelViewMatrix * vec4(in_Normal, 0.0)).xyz;
	vec3 B = normalize(cross(T, N));

	TBNinverse = mat3(T, B, N);
}