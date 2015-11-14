// simple vertex shader

#version 330

uniform float in_LifetimeRef;

layout(location = 0) in vec2 in_Position;
layout(location = 1) in float in_Lifetime;

out float alpha;
out float beta;

void main()
{
	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);

	alpha = in_Lifetime / in_LifetimeRef;

	if(in_Lifetime < in_LifetimeRef / 2.0)
		beta = alpha * 2.0;
	else
		beta = (1.0 - alpha) * 2.0;

}
