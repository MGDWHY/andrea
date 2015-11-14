// simple fragment shader

// 'time' contains seconds since the program was linked.

uniform float time;
uniform sampler2D velocity;

void main()
{
	vec3 vel = texture2D(velocity, gl_TexCoord[0].st).xyz * 2.0 - 1.0;
	vec3 pos = vel * fract(time);

	pos = (pos + 1.0) / 2.0;

	
	gl_FragColor = vec4(pos, 1.0);
}
