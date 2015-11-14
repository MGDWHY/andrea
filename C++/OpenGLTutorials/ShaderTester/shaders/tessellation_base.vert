#version 330

uniform mat4 in_ModelViewMatrix;

layout(location = 0) in vec4 in_Vertex;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

out vec3 normal;
out vec2 texCoord;

void main() {
	gl_Position = in_ModelViewMatrix * in_Vertex;
	normal = in_Normal;
	texCoord = in_TexCoord;		
}