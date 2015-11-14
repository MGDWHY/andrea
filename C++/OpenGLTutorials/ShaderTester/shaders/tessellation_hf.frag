#version 330

uniform sampler2D in_Terrain;

in vec2 fgTexCoord;

out vec4 gl_FragColor;

void main() {
	gl_FragColor = texture(in_Terrain, fgTexCoord);
}