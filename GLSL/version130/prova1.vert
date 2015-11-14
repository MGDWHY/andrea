// simple vertex shader

#version 130

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec3 in_Position;

void main()
{
	vec4 pos = vec4(in_Position, 1.0);
	gl_Position = projectionMatrix * modelViewMatrix * pos;
	gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
}
