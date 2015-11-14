#version 420

uniform mat4 in_ProjectionMatrix;

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

flat in vec3 gsNormal[3];

void main() {
	for(int i = 0; i < 3; i++) {
		gl_Position = in_ProjectionMatrix * gl_in[i].gl_Position;
		EmitVertex();
		
		gl_Position = in_ProjectionMatrix * vec4(gl_in[i].gl_Position.xyz + gsNormal[i] * 0.2, 1.0);
		EmitVertex();
		
		EndPrimitive();
	}
}