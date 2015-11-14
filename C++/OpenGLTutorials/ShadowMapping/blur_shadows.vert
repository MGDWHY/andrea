#version 330

uniform mat4 in_ProjectionMatrix;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

out vec2 texCoord;

void main() {
	gl_Position = in_ProjectionMatrix * vec4(in_Position, 1.0);
	texCoord = in_TexCoord;
}