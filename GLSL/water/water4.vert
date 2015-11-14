// water shader v4 (bump mapping + superficie parametrica)

uniform float time;

uniform bvec4 lgt_0123;

varying vec3 lgtVec[4];
varying vec3 spotDir[4];

varying vec3 eyeVec;

varying vec2 windowPosition;

void main()
{

	// WAVE AMPLITUDES
	float a1 = 0.08;
	float a2 = 0.05;
	
	//WAVE FREQS
	float w1 = 1.0;
	float w2 = 1.7;

	// WAVE PHASES
	float t1 = -2.0;
	float t2 = 3.2;
	
	// SURFACE PARAMETERS
	float s = gl_Vertex.x;
	float t = gl_Vertex.z;

	vec3 pos = vec3(s, gl_Vertex.y + a1*sin(w1*s+time*t1) + a2*sin(w2*(s+t*0.5)+time*t2),  t );
	vec3 position = (gl_ModelViewMatrix * vec4(pos,1.0)).xyz;

	vec3 ds = vec3(1.0, a1*w1*cos(w1*s+time*t1) + a2*w2*cos(w2*(s+t*0.5)+time*t2),  0.0  );
	vec3 dt = vec3(0.0, 0.5*a2*w2*cos(w2*(s+t/2.0) + time*t2), 1.0  );

	gl_Position    = gl_ProjectionMatrix * vec4(position, 1.0);
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;


	vec4 v = gl_ProjectionMatrix * vec4(position, 1.0);
	windowPosition = ((v.xy / v.w) + 1.0) / 2.0;
	//windowPosition.y = -windowPosition.y;
	
	//-----------------TANGENT SPACE------------------------------
	vec3 norm = gl_NormalMatrix * normalize(cross(dt,ds));
	vec3 tang = gl_NormalMatrix * normalize(ds);
	vec3 binor = gl_NormalMatrix * normalize(dt);
	
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
