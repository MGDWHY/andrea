// simple fragment shader
// 'time' contains seconds since the program was linked.

uniform float time;
uniform sampler2D texture;

const float sum = 1.0;

const mat3 kernel = mat3( 
						vec3(-1.0, 		-1.0, 		-1.0),
						vec3(-1.0,		8.0, 		-1.0),
						vec3(-1.0, 		-1.0, 		-1.0)
					) / sum;

vec4 convolve(vec2 coords, float prec) {
	vec4 result = vec4(0.0);	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			result += texture2D(texture, coords.st + vec2( float(i-1) * prec, float(j-1) * prec)) * kernel[i][j];
	return result;
}

vec4 grayScale(vec4 color) {
	
	return vec4(vec3( (color.r + color.g + color.b) * 0.333 ), color.a);
}

vec4 invert(vec4 color) {
	return vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
}

void main()
{
	gl_FragColor = invert(grayScale(convolve(gl_TexCoord[0].st, 0.005)));
}
