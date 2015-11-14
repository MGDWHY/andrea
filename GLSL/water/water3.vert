// water shader v3 (bump mapping)

uniform float time;

uniform bvec4 lgt_0123;

uniform vec3 tangent;

varying vec3 lgtVec[4];
varying vec3 spotDir[4];

varying vec3 eyeVec;


void main()
{


	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec3 position = (gl_ModelViewMatrix * gl_Vertex).xyz;
	
	//-----------------TANGENT SPACE------------------------------
	vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
	vec3 tang = normalize(gl_NormalMatrix * tangent);
	vec3 binor = normalize(cross(norm, tang));
	
	//-----------------EYE VECTOR------------------------------	
	eyeVec.x = dot(-position.xyz, tang);
	eyeVec.y = dot(-position.xyz, binor);
	eyeVec.z = dot(-position.xyz, norm);
	
	for(int i = 0; i < 4; i++) {
		vec3 tmpVec = gl_LightSource[i].position.xyz - position;
		
		//-----------------LIGHT VECTOR------------------------------
		lgtVec[i].x = dot(tmpVec, tang);
		lgtVec[i].y = dot(tmpVec, binor);
		lgtVec[i].z = dot(tmpVec, norm);

		tmpVec = gl_LightSource[i].spotDirection;
		
		//-----------------SPOT VECTOR------------------------------
		spotDir[i].x = dot(tmpVec, tang);
		spotDir[i].y = dot(tmpVec, binor);
		spotDir[i].z = dot(tmpVec, norm);		
	}
}
