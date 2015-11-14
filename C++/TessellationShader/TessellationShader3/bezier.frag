// bezier.frag
#version 420

smooth in vec3 fsNormal;

out vec4 gl_FragColor;

void main() {

	vec3 normal = normalize(fsNormal);
	
	float diffuse = max(0.0, dot(normal, vec3(0, 0, 1)));
	
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0) * diffuse;
}
