#version 420

uniform mat4 in_ModelViewMatrix;
uniform mat4 in_ProjectionMatrix;

layout(location = 0) in vec3 in_Position;

void main() {
	gl_Position = in_ProjectionMatrix * in_ModelViewMatrix * vec4(in_Position, 1.0);
}