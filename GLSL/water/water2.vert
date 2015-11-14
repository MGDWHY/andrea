// water shader v2 (superficie parametrica)

uniform float time;

varying float s, t;

varying vec3 position;

const float PI = 3.141592;

void main()
{

	float a1 = 0.15;
	float a2 = 0.1;
	float w1 = 1.0;
	float w2 = 1.7;
	float t1 = 2.0;
	float t2 = 3.0;
	
	s = gl_Vertex.x;
	t = gl_Vertex.z;

	vec3 pos = vec3(s, gl_Vertex.y + a1*sin(w1*s+time*t1) + a2*sin(w2*(s+t*0.5)+time*t2),  t );
	position = (gl_ModelViewMatrix * vec4(pos, 1.0)).xyz;

	gl_Position    = gl_ProjectionMatrix * vec4(position, 1.0);
	gl_TexCoord[0].st = gl_MultiTexCoord0.st + vec2(time/10.0, time/10.0);
	gl_FrontColor = gl_Color;
}
