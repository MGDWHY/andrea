// shadows.frag

#version 330

out vec4 gl_FragColor;

in vec4 position;

void main() {
	gl_FragColor.r = position.z / position.w;
}