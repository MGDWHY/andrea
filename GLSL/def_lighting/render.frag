#version 130

const int MAX_LIGHTS = 20;

struct light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec3 spotDirection;
	float spotCutoff;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float maxDistance;
};

uniform light lights[MAX_LIGHTS];

uniform sampler2D in_TexDiffuse;
uniform sampler2D in_TexNormal;
uniform sampler2DShadow in_TexDepth;

uniform mat4 in_InverseProjMatrix;

/*
	DEBUG MODE
	0 - scene output
	1 - diffuse 
	2 - normal
	3 - gloss
	4 - depth
*/
uniform int in_DebugMode; 

in vec2 texCoord;
in vec2 windowPosition;

vec4 lighting(light lgt, in vec3 norm, in vec3 lgtVector, in vec3 eyeVector, in vec4 pixDiffuse, float pixShininess, float pixPower) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = vec3(lgtVector); // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	//-----------------DIFFUSE FACTOR------------------------------
	float diffFactor = max(0.0, dot(norm, lightVector));
	
	//-----------------SPOT LIGHT CUTOFF TEST------------------------------
	/*float spotCosCutoff = cos(lgt.spotCutoff);
	if(spotCosCutoff > 0.0) {
		float angle = dot(-lightVector, normalize(lgt.spotDirection));
		if(angle < spotCosCutoff)
			diffFactor = 0.0;
	}*/
	
	//-----------------LIGHT ATTENUATION------------------------------

	float attenuation = 1.0 / ( lgt.constantAttenuation +	
				lgt.linearAttenuation * dist +	
				lgt.quadraticAttenuation * dist * dist);
	
	//-----------------DIFFUSE COLOR------------------------------
	vec4 diffuse = lgt.diffuse  * diffFactor * attenuation;
	
	//-----------------SPECULAR COLOR------------------------------
	if(diffFactor != 0.0) {
		vec3 eye = normalize(eyeVector);
		vec3 halfVector = normalize(eye + lightVector);
		float specFactor = max(0.0, dot(halfVector, norm));

		specFactor = pow(specFactor, pixShininess) * clamp(pixPower, 0.0, 1.0);

		specular = lgt.specular * specFactor * attenuation;
	}
	
	//-----------------PIXEL COLOR------------------------------
 
	return (lgt.ambient + diffuse) * pixDiffuse + specular;
	

}

void main()
{
	float depth = texture(in_TexDepth, vec3(texCoord, 0.0));
	vec3 ncsPos = vec3(windowPosition, depth * 2.0 - 1.0);
	vec4 vcsPos = in_InverseProjMatrix * vec4(ncsPos, 1.0);
	
	vcsPos /= vcsPos.w;	
	
	vec4 finalColor = vec4(0.0);

	vec4 pixDiffuse = texture(in_TexDiffuse, texCoord);
	vec4 NG = texture(in_TexNormal, texCoord);
	vec3 normal = normalize(NG.xyz * 2.0 - 1.0);
	float gloss = NG.a;	

	for(int i = 0; i < MAX_LIGHTS; i++) {
		vec3 lightVector = lights[i].position.xyz - vcsPos.xyz;
		finalColor += lighting(lights[i], normal, lightVector, -vcsPos.xyz, pixDiffuse, 128.0, gloss); 
	}	

	if(in_DebugMode == 0)
		gl_FragColor = finalColor;
	else if(in_DebugMode == 1)
		gl_FragColor = pixDiffuse;
	else if(in_DebugMode == 2)
		gl_FragColor = vec4(normal, 1.0);
	else if(in_DebugMode == 3)
		gl_FragColor = vec4(gloss, gloss, gloss, 1.0);
	else if(in_DebugMode == 4)
		gl_FragColor = vec4(depth, depth, depth, 1.0);

}	
