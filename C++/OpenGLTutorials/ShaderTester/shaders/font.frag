#version 330

uniform sampler2D in_Font;

smooth in vec2 texCoord;
flat in vec4 color_geom;

out vec4 gl_FragColor;

void main() {
	vec4 pix = texture(in_Font, texCoord);
	gl_FragColor = vec4(color_geom.xyz, pix.r);
}