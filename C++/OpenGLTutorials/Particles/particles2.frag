#version 330

uniform sampler2D in_Sprite;


flat in vec3 color;
noperspective in vec2 texCoords;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = vec4(color, 0.5) * texture(in_Sprite, texCoords);
}
