// Per-pixel lighting 

uniform float time;

varying vec3 position, normal;
varying vec4 color;

const float PI = 3.141592;

void main()
{
	float s = gl_Vertex.x;
	float t = gl_Vertex.z;
	
	float a1 = 0.1;
	float a2 = 0.05;
	float w1 = 5.0;
	float w2 = 7.0;
	float t1 = 2.0;
	float t2 = 3.0;
	

	vec3 pos = vec3(s, gl_Vertex.y + a1*sin(w1*s+time*t1) + a2*sin(w2*(s+t/2)+time*t2),  t );
	
	vec3 ds = vec3(1.0, a1*w1*cos(w1*s+time*t1) + a2*w2*cos(w2*(s+t/2)+time*t2),  0.0  );
	vec3 dt = vec3(0.0, a2*w2*cos(w2*(s+t/2) + time*t2), 1.0  );

	normal = gl_NormalMatrix * normalize(cross(dt,ds));
	position = gl_ModelViewMatrix * vec4(pos, 1.0);

	gl_Position    = gl_ModelViewProjectionMatrix * vec4(pos,1.0);
	gl_TexCoord[0].st = gl_MultiTexCoord0.st + vec2(time/10, time/10);
	gl_FrontColor = gl_Color;
}
