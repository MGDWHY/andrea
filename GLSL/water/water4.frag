// water shader v4 (bump mapping + superficie parametrica)

uniform float time;

uniform bool useTexture;
uniform float reflectivePower;
uniform sampler2D reflections;
uniform sampler2D normalMap1;
uniform sampler2D normalMap2;
uniform bvec4 lgt_0123;

varying vec3 spotDir[4];
varying vec3 lgtVec[4];

varying vec2 windowPosition;

varying vec3 eyeVec;

vec4 lighting(in int index, in vec3 norm, in vec3 lgtVector, in vec3 eyeVector, in vec3 spotVector) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = vec3(lgtVector); // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	//-----------------DIFFUSE FACTOR------------------------------
	float diffFactor = max(0.0, dot(norm, lightVector)); // diffuse color factor
	
	//-----------------SPOT LIGHT CUTOFF TEST------------------------------
	if(gl_LightSource[index].spotCosCutoff > 0.0) {
		float angle = dot(-lightVector, normalize(spotVector));
		if(angle < gl_LightSource[index].spotCosCutoff)
			diffFactor = 0.0;
	}
	
	//-----------------LIGHT ATTENUATION------------------------------
	float attenuation = 1.0 / ( gl_LightSource[index].constantAttenuation +	
								gl_LightSource[index].linearAttenuation * dist +	
								gl_LightSource[index].quadraticAttenuation * dist * dist);	
	
	//-----------------DIFFUSE COLOR------------------------------
	vec4 diffuse = gl_LightSource[index].diffuse  * diffFactor * attenuation;
	
	//-----------------SPECULAR COLOR------------------------------
	if(diffFactor != 0.0) {
		vec3 eye = normalize(eyeVector);
		vec3 halfVector = normalize(eye + lightVector);
		float specFactor = max(0.0, dot(halfVector, norm));

		specFactor = pow(specFactor, gl_FrontMaterial.shininess) * clamp(reflectivePower, 0.0, 1.0);

		specular = gl_LightSource[index].specular * specFactor;
	}
	
	vec4 pixel;
	
	//-----------------PIXEL COLOR------------------------------
	if(useTexture)
		pixel = texture2D(reflections, windowPosition.xy + norm.xy * 0.02);
	else
		pixel = gl_Color; // use default color

	return (gl_LightSource[index].ambient + diffuse) * pixel + specular;
}

void main()
{
	vec3 norm1 = normalize(texture2D(normalMap1, gl_TexCoord[0].st + vec2(time/15.0, 0.0)).xyz * 2.0 - 1.0);
	vec3 norm2 = normalize(texture2D(normalMap2, gl_TexCoord[0].st + vec2(-time/27.0, -time/42.0)).xyz * 2.0 - 1.0);

	vec3 norm = normalize(norm1 + norm2);
	
	vec4 color = vec4(0.0);

	for(int i = 0; i < 4; i++)
		if(lgt_0123[i])
			color += lighting(i, norm, lgtVec[i], eyeVec, spotDir[i]);

	gl_FragColor = color;
}
