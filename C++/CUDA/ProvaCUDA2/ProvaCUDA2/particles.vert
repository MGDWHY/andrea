#version 330

uniform mat4 in_ModelViewProjectionMatrix;

layout(location = 0) in vec2 in_Position;

void main() {
	gl_Position = in_ModelViewProjectionMatrix * vec4(in_Position.x, in_Position.y, 0.0, 1.0);
}