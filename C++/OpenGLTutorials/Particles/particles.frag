#version 330

in float alpha;
in float beta;

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = vec4(1.0, 1.0 - alpha, 0.0, beta);
}
