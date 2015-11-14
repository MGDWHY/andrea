// Per-pixel lighting 

// 4 lights

uniform bvec4 lgt_0123;
uniform bvec4 clip_0123;

uniform bool useTexture;

uniform float reflectivePower;

uniform sampler2D texture;

varying vec3 position, normal;

const float DEG_TO_RAD = 3.141592/180.0;

vec4 lighting(in int index, in vec3 norm, in vec3 lgtVector, in vec3 eyeVector, in vec3 spotVector) {

	vec4 specular = vec4(0.0); // specular color

	vec3 lightVector = vec3(lgtVector); // vector from vertex to light

	float dist = length(lightVector); // distance from the light

	lightVector = normalize(lightVector); // normalized light vector
	
	//-----------------DIFFUSE FACTOR------------------------------
	float diffFactor = max(0.0, dot(norm, lightVector));
	
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
		pixel = texture2D(texture, gl_TexCoord[0].st); // use texture color
	else
		pixel = gl_Color; // use default color

	return (gl_LightSource[index].ambient + diffuse) * pixel + specular;
}

void main()
{
	vec3 norm = normalize(normal);
	vec4 finalColor = vec4(0.0);

	for(int i = 0; i < 4; i++)
		if(clip_0123[i]) {
			float res = dot(position.xyz, gl_ClipPlane[i].xyz) + gl_ClipPlane[i].w;
			if(res <= 0.0)
				discard; 
		}


	for(int i = 0; i < 4; i++)	
		if(lgt_0123[i])
			finalColor += lighting(i, norm, gl_LightSource[i].position.xyz - position, -position, gl_LightSource[i].spotDirection);


	gl_FragColor = finalColor;
}


