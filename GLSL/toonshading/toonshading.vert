// Per-pixel lighting 

varying vec3 position, normal;


void main()
{
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	position = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = gl_NormalMatrix * gl_Normal;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
