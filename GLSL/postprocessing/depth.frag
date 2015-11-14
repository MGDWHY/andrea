// simple fragment shader
// 'time' contains seconds since the program was linked.

uniform sampler2D texture;
uniform sampler2D zbuffer;

uniform float stepS;
uniform float stepT;

const float sum = 9.0;

vec4 depthConvolve(vec2 coords) {
	vec4 result = vec4(0.0);
	
	float depth = texture2D(zbuffer, coords.st).x;

	mat3 kernel = mat3(depth, depth, depth, depth, 10.0 - depth * 9.0, depth, depth, depth, depth) / sum;	

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			result += texture2D(texture, coords.st + vec2( float((i-1) * 3 + j) * stepS, 0)) * kernel[i][j];
	
	return result;
}

void main()
{
	gl_FragColor = depthConvolve(gl_TexCoord[0].st);
}
