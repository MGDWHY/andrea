#version 330



uniform mat4 in_ModelViewMatrix;

layout(location = 0) in vec4 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec2 in_HfCoord;

out vec3 normal;
out vec2 texCoord;
out vec2 hfCoord;

void main() {
	vec4 position = in_ModelViewMatrix * in_Vertex;
	gl_Position = position;
	normal = in_Normal;
	texCoord = in_TexCoord;
	hfCoord = in_HfCoord;
}