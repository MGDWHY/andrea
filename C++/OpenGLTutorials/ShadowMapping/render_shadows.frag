// shadows.frag

#version 330

uniform mat4 in_LightProjectionMatrix;

uniform sampler2D in_ShadowsR;

out vec4 gl_FragColor;

in vec4 shadowCoords;

void main() {

	vec4 ncsShadowCoords = in_LightProjectionMatrix * shadowCoords;
	
	ncsShadowCoords /= ncsShadowCoords.w;
	
	float zShadow = texture(in_ShadowsR, clamp((ncsShadowCoords.xy+1.0)/2.0, 0.0, 1.0)).r;
	
	if(ncsShadowCoords.z < zShadow+0.0005)
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	else
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
