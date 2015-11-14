#version 420

uniform mat4 in_ProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

flat in vec3 gsNormal[3];
smooth out vec3 fsNormal;

void main() {
	for(int i = 0; i < 3; i++) {
		gl_Position = in_ProjectionMatrix * gl_in[i].gl_Position;
		fsNormal = gsNormal[i];
		EmitVertex();
	}
	EndPrimitive();
}