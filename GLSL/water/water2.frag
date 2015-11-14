// water shader v2 (superficie parametrica)


uniform float time;

uniform bvec4 lgt_0123;

uniform bool useTexture, useShininessMap;

uniform float reflectivePower;

uniform sampler2D texture;
uniform sampler2D shininessMap;

varying vec3 position, normal;
varying float s, t;

const float DEG_TO_RAD = 3.141592/180.0;
vec4 calculateLightContribution(in int index, in vec3 norm, in vec3 lgtVector, in vec3 eyeVector, in vec3 spotVector) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = vec3(lgtVector); // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	float diffFactor = max(0.0, dot(norm, lightVector)); // diffuse color factor
	
	// spotlight
	if(gl_LightSource[index].spotCosCutoff > 0.0) {
		float angle = dot(-lightVector, normalize(spotVector));
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
		vec3 eye = normalize(eyeVector); // vector to camera
		vec3 halfVector = normalize(eye + lightVector);
		float specFactor = max(0.0, dot(halfVector, norm));

		specFactor = pow(specFactor, gl_FrontMaterial.shininess) * clamp(reflectivePower, 0.0, 1.0);

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
	float a1 = 0.15;
	float a2 = 0.1;
	float w1 = 1.0;
	float w2 = 1.7;
	float t1 = 2.0;
	float t2 = 3.0;

	vec3 ds = vec3(1.0, a1*w1*cos(w1*s+time*t1) + a2*w2*cos(w2*(s+t*0.5)+time*t2),  0.0  );
	vec3 dt = vec3(0.0, 0.5*a2*w2*cos(w2*(s+t/2.0) + time*t2), 1.0  );
	vec3 norm = gl_NormalMatrix * normalize(cross(dt,ds));
	

	vec4 finalColor = vec4(0.0);

	for(int i = 0; i < 4; i++)
		if(lgt_0123[i])
			finalColor += calculateLightContribution(i, norm, gl_LightSource[i].position.xyz - position.xyz, -position, gl_LightSource[i].spotDirection);
	
	gl_FragColor = finalColor;
}


