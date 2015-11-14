// simple vertex shader

#version 130

in vec2 in_TexCoord;

out vec2 texCoord;
out vec2 windowPosition;

void main()
{
	vec3 winPos = vec3(in_TexCoord * 2.0 - 1.0, 0.0);
	gl_Position = vec4(winPos, 1.0);
	
	windowPosition = winPos.xy;
	texCoord = in_TexCoord;
}
