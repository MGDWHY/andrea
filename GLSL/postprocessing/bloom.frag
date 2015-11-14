// simple fragment shader
// 'time' contains seconds since the program was linked.

uniform sampler2D texture;

uniform float stepS;
uniform float stepT;

const float sum = 6.0;

const mat3 kernel = mat3( 
						vec3(1.0/sum, 1.0/sum, 1.0/sum),
						vec3(1.0/sum, 1.0/sum, 1.0/sum),
						vec3(1.0/sum, 1.0/sum, 1.0/sum)
					);

vec4 bloomConvolve(vec2 coords) {
	vec4 result = vec4(0.0);
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			result += texture2D(texture, coords.st + vec2( float(i-1) * stepS, float(j-1) * stepT) ) * kernel[i][j];
	
	result = pow(result, 2.0) + texture2D(texture, coords.st);

	return result;
}

void main()
{
	gl_FragColor = bloomConvolve(gl_TexCoord[0].st);
}
