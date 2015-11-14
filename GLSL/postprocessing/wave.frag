// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;
uniform sampler2D texture;

void main()
{
	float t = gl_TexCoord[0].t + (sin(gl_TexCoord[0].s*100 + time*3)*0.002);
	float s = gl_TexCoord[0].s + (sin(gl_TexCoord[0].t*50 + time*4)*0.001);
	vec4 pixel = texture2D(texture, vec2(s, t));
	gl_FragColor = pixel;
}
