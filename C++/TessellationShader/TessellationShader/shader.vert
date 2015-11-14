#version 420

uniform vec2 in_Viewport;
uniform mat4 in_ModelViewMatrix;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

flat out vec2 vsTexCoord;

void main() {
	gl_Position = in_ModelViewMatrix * vec4(in_Position, 1.0);
	vsTexCoord = in_TexCoord;
}