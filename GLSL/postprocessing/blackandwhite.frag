// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;

uniform sampler2D texture;

const mat3 bwMat = mat3(
						0.333, 0.333, 0.333,
						0.333, 0.333, 0.333, 
						0.333, 0.333, 0.333
					);

void main()
{
	vec3 noise = noise3(1);
	vec3 pixel = bwMat * texture2D(texture, gl_TexCoord[0].st).rgb;;
	gl_FragColor = vec4( pixel, 1.0) ;
}
