// simple fragment shader

// 'time' contains seconds since the program was linked.
varying vec3 normal, eye;

const vec3 white = vec3(1.0, 1.0, 1.0);

void main()
{
	float val = max(0.0, dot(normalize(normal), normalize(eye)));
	float valPowah = pow(val, 3.0);
	/*if(val > 0.999)
		gl_FragColor = vec4(white, 1.0);
	else*/
		gl_FragColor = vec4(mix(gl_Color.rgb, white, valPowah), valPowah);
}
