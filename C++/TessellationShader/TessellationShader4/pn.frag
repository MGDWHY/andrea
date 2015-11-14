#version 420

out vec4 gl_FragColor;

smooth in vec3 tesPosition;
smooth in vec3 tesNormal;

void main() {
	
	vec3 L = normalize(-tesPosition);
	vec3 N = normalize(tesNormal);
	float diffuseFactor = max(0.0, dot(L, N));
	
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0) * diffuseFactor;
}