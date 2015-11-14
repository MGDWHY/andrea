#version 420

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

flat out vec3 vsNormal;

void main() {
	gl_Position = vec4(in_Position, 1.0);
	vsNormal = in_Normal;
}