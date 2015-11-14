#version 330

layout(location = 0) in vec2 in_Vertex;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in int in_AsciiCode;

out vec4 color;
out int asciiCode;

void main() {
	gl_Position = vec4(in_Vertex.x, in_Vertex.y, 0.0, 1.0);
	color = in_Color;
	asciiCode = in_AsciiCode;	
}