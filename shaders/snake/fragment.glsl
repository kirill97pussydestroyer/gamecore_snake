#version 130

uniform vec3 color;

void main()
{
	gl_FragData[0] = vec4(color, 1);
}
