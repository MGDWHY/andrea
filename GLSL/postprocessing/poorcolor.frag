// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;

uniform sampler2D texture;

float discFactor(float f) {
	if(f < 0.1)
		return 0.0;
	else if(f < 0.3)
		return 0.3;
	else if(f < 0.6)
		return 0.6;
	else if(f < 0.8)
		return 0.9;
	else
		return 1.0;
}

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].st);
	gl_FragColor =  vec4(discFactor(color.r), discFactor(color.g), discFactor(color.b), color.a);
}
