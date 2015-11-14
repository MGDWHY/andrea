// Per-pixel lighting 

// Supports up to 4 point lights

uniform bvec4 lgt_0123;

uniform bool useTexture;
uniform bool useShininessMap;

uniform float reflectivePower;

uniform sampler2D texture;
uniform sampler2D shininessMap;

varying vec3 position, normal;

varying vec4 color;

const float DEG_TO_RAD = 3.141592/180.0;

vec4 calculateLightContribution(in int index, in vec3 norm) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = gl_LightSource[index].position.xyz - position; // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	float diffFactor = max(0.0, dot(norm, lightVector)); // diffuse color factor
	
	// spotlight
	if(gl_LightSource[index].spotCosCutoff > 0.0) {
		float angle = dot(-lightVector, normalize(gl_LightSource[index].spotDirection));
		if(angle < gl_LightSource[index].spotCosCutoff)
			diffFactor = 0.0;
	}
	
	// attenuation
	float attenuation = 1.0 / ( gl_LightSource[index].constantAttenuation +	
								gl_LightSource[index].linearAttenuation * dist +	
								gl_LightSource[index].quadraticAttenuation * dist * dist);	
	
	// diffuse color
	vec4 diffuse = gl_LightSource[index].diffuse  * diffFactor * attenuation;
	
	// specular color
	if(diffFactor != 0.0) {
		vec3 eyeVector = normalize(-position); // vector to camera
		vec3 halfVector = normalize(eyeVector + lightVector);
		float specFactor = max(0.0, dot(halfVector, norm));

		specFactor = pow(specFactor, gl_FrontMaterial.shininess) * clamp(reflectivePower, 0.0, 1.0);
	
		if(useShininessMap)
			specFactor *= texture2D(shininessMap, gl_TexCoord[0].st).r;

		specular = gl_LightSource[index].specular * specFactor;
	}
	
	vec4 pixel;

	if(useTexture)
		pixel = texture2D(texture, gl_TexCoord[0].st);
	else
		pixel = gl_Color;

	return (gl_LightSource[index].ambient + diffuse) * pixel + specular;

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


