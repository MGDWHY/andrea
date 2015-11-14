// simple vertex shader

#version 330

layout(location = 0) in vec2 in_Position;
layout(location = 1) in float in_Lifetime;

out float lifeTime;

void main()
{
	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	lifeTime = in_Lifetime;
}
