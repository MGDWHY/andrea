#version 330

const float kernel[9] = float[9] ( 1, 33, 403, 1808, 2980, 1808, 403, 33, 1 );

uniform float in_Step;
uniform sampler2D in_BlurredShadows;
uniform bool in_ConvolveH;

out vec4 gl_FragColor;

in vec2 texCoord;

void main() {
	vec3 color = vec3(0.0);
	
	if(in_ConvolveH)
		for(int i = -4; i <= 4; i++)
			color += kernel[i + 4] * texture(in_BlurredShadows, texCoord + vec2(i * in_Step, 0.0)).rgb;
	else
		for(int i = -4; i <= 4; i++)
			color += kernel[i + 4] * texture(in_BlurredShadows, texCoord + vec2(0.0, i * in_Step)).rgb;	
	
	gl_FragColor = vec4(color / 7470, 1.0);
}