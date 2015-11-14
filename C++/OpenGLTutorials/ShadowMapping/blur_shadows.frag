#version 330

const mat3 kernel = mat3(1.0, 1.0, 1.0, 
						1.0, 1.0, 1.0, 
						1.0, 1.0, 1.0) / 9.0;

uniform float in_StepX, in_StepY;
uniform sampler2D in_AliasedShadows;

out vec4 gl_FragColor;

in vec2 texCoord;

void main() {
	vec3 color = vec3(0.0);
	for(int j = -1; j <= 1; j++)
		for(int i = -1; i <= 1; i++)
			color += kernel[j+1][i+1] * texture(in_AliasedShadows, texCoord + vec2(i * in_StepX, i * in_StepY)).rgb;
	
	gl_FragColor = vec4(color, 1.0);
}