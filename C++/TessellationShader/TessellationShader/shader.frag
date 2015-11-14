#version 420

uniform sampler2D in_Texture;
uniform sampler2D in_NormalMap;

out vec4 gl_FragColor;

smooth in mat3 tesInverseTBN;
smooth in vec3 tesPosition;
smooth in vec2 tesTexCoord;

void main() {
	
	vec3 L = normalize(-tesPosition);
	vec3 N = tesInverseTBN * (texture(in_NormalMap, tesTexCoord).xyz * 2.0 - 1.0);
	
	float diffuseFactor = max(0.0, dot(L, N));
	float specularFactor = pow(max(0.0, dot(L, N)), 128);
	
	// gl_FragColor = vec4(texture(in_Texture, tesTexCoord).rgb, 1.0) * diffuseFactor;
	gl_FragColor = texture(in_Texture, tesTexCoord) * diffuseFactor + vec4(1.0) * specularFactor;
}