#version 330

uniform vec2 in_WCStoPixel;
uniform float in_PointSize;
uniform float in_LifetimeRef;
uniform float in_Time;

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in float lifeTime[1];

flat out vec3 color;
noperspective out vec2 texCoords;

void main( void )
{
	
	float f1 = abs(cos(in_Time));
	float f2 = abs(sin(in_Time)) / 2.0 + 0.5;

	float alpha = lifeTime[0] / in_LifetimeRef;
	float beta;

	if(lifeTime[0] < in_LifetimeRef / 2.0)
		beta = alpha * 2.0;
	else
		beta = (1.0 - alpha) * 2.0;
	
	
			
	vec4 pos = gl_in[0].gl_Position;
	
	
	color = vec3(abs(pos.x) * f1, abs(pos.y), f2);

	vec2 stepVec = in_WCStoPixel * in_PointSize;
	
	texCoords = vec2(0.0, 1.0);
	gl_Position = vec4(pos.x - stepVec.x, pos.y - stepVec.y, 0.0, 1.0);
	EmitVertex();
	
	texCoords = vec2(1.0, 1.0);
	gl_Position = vec4(pos.x + stepVec.x, pos.y - stepVec.y, 0.0, 1.0);
	EmitVertex();
	
	texCoords = vec2(0.0, 0.0);
	gl_Position = vec4(pos.x - stepVec.x, pos.y + stepVec.y, 0.0, 1.0);
	EmitVertex();

	EndPrimitive();
	
	texCoords = vec2(1.0, 1.0);
	gl_Position = vec4(pos.x + stepVec.x, pos.y - stepVec.y, 0.0, 1.0);
	EmitVertex();

	texCoords = vec2(1.0, 0.0);
	gl_Position = vec4(pos.x + stepVec.x, pos.y + stepVec.y, 0.0, 1.0);
	EmitVertex();

	texCoords = vec2(0.0, 0.0);
	gl_Position = vec4(pos.x - stepVec.x, pos.y + stepVec.y, 0.0, 1.0);
	EmitVertex();

	EndPrimitive();
}
