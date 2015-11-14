// Per-pixel lighting 

// Supports up to 4 point lights

uniform bvec4 lgt_0123;
uniform sampler2D texture;


varying vec3 position, normal;

float sample(float factor) {
	if(factor < 0.2)
		return 0.0;
	else if(factor < 0.5)
		return 0.75;
	else
		return 1.0;
}

vec4 calculateLightContribution(in int index, in vec3 norm) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = gl_LightSource[index].position.xyz - position; // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	float diffFactor = max(0.0, dot(norm, lightVector)); // diffuse color factor
	
	// attenuation
	float attenuation = 1.0 / ( gl_LightSource[index].constantAttenuation +	
								gl_LightSource[index].linearAttenuation * dist +	
								gl_LightSource[index].quadraticAttenuation * dist * dist);	
	
	//diffuse color
	vec4 diffuse = gl_LightSource[index].diffuse  * sample(diffFactor) * attenuation;

	if(diffFactor != 0.0) {
		vec3 eyeVector = normalize(-position); // camera
		vec3 halfVector = normalize(eyeVector + lightVector);
		float specFactor = max(0.0, dot(halfVector, norm));
		specFactor = pow(specFactor, gl_FrontMaterial.shininess);
		if(specFactor > 0.9)
			specular = gl_LightSource[index].specular * sample(specFactor);
	}
		
	

	return gl_LightSource[index].ambient + diffuse * gl_Color + specular;
}

void main()
{
	vec3 norm = normalize(normal);
	vec4 finalColor = vec4(0.0);

	for(int i = 0; i < 4; i++)
		if(lgt_0123[i])
			finalColor += calculateLightContribution(i, norm);
	
	gl_FragColor = finalColor;
}


