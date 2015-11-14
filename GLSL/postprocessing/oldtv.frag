// simple fragment shader

uniform float rand;
uniform float scale;

uniform sampler2D texture;
uniform sampler2D noise;

const mat3 bwMat = mat3(
						0.333, 0.333, 0.333,
						0.333, 0.333, 0.333, 
						0.333, 0.333, 0.333
					);

void main()
{
	vec3 pixel = bwMat * texture2D(texture, gl_TexCoord[0].st).rgb + texture2D(noise, gl_TexCoord[0].st + rand).rgb * scale;
	gl_FragColor = vec4( pixel, 1.0) ;
}
