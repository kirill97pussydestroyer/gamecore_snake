#version 130

in vec2 pos;

out vec2 uv;

void main()
{
	uv = pos / 2.0 + vec2(0.5);
	gl_Position = vec4(pos, 0.0, 1.0);
}
