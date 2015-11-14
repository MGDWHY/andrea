// simple vertex shader

uniform vec3 normal;

varying float factor;

void main()
{
	vec3 lightToPos = (gl_ModelViewMatrix * gl_Vertex).xyz - gl_LightSource[1].position.xyz;
	vec3 lv = normalize(lightToPos);
	vec3 n = -normalize(gl_NormalMatrix*normal);
	factor = dot(n, lv);
	
	gl_Position = gl_ProjectionMatrix * vec4(gl_LightSource[1].position.xyz + lv * 4.99 / factor, 1.0);

	gl_FrontColor  = gl_Color;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
