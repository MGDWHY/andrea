// simple fragment shader

// 'time' contains seconds since the program was linked.
uniform float time;

varying float factor;

void main()
{
	gl_FragColor = vec4(0.0,0.0,0.0,abs(factor));
}
