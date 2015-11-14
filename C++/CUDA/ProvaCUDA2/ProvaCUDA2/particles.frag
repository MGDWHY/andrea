#version 330

uniform vec4 in_Color;

out vec4 gl_FragColor;

void main() {
	gl_FragColor = in_Color;
}