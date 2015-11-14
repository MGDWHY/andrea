#version 330

uniform bool in_UseTexture;

uniform vec4 in_Color;
uniform sampler2D in_TexDiffuse;
uniform sampler2D in_TexNormal;

in mat3 TBNinverse;
in vec2 texCoords;

void main() {

	vec4 NG = texture(in_TexNormal, texCoords);
	
	vec3 normal = TBNinverse * (NG.xyz * 2.0 - 1.0);

	float gloss = NG.a;

	gl_FragData[0] = vec4((normal+1.0)/2.0, gloss);
	

	if(!in_UseTexture)
		gl_FragData[1] = in_Color;
	else
		gl_FragData[1] = texture(in_TexDiffuse, texCoords);
		
}