// shadows.vert

#version 330

uniform mat4 in_LightViewMatrix;
uniform mat4 in_LightProjectionMatrix;

uniform mat4 in_ViewMatrix;
uniform mat4 in_ModelMatrix;
uniform mat4 in_ProjectionMatrix;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

out vec4 shadowCoords;

void main() {
	// projection of the current vertex position
	gl_Position = in_ProjectionMatrix * in_ViewMatrix * in_ModelMatrix * vec4(in_Position, 1.0);
	
	// vertex coords fron the light pov
	shadowCoords = in_LightViewMatrix * in_ModelMatrix * vec4(in_Position, 1.0);
}