// simple vertex shader

#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec3 in_Position;

void main()
{
	vec4 pos = vec4(in_Position, 1.0);
	gl_Position = projectionMatrix * modelViewMatrix * pos;
}
