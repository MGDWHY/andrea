#version 330

#define BG_RATIO_Y 1.0

layout(location = 0) in vec2 in_TexCoord;

smooth out vec2 texCoord;

void main() {
	gl_Position = vec4(in_TexCoord.x * 2.0 - 1.0, 1.0 - in_TexCoord.y * BG_RATIO_Y, 0.0, 1.0);
	texCoord = in_TexCoord;
}