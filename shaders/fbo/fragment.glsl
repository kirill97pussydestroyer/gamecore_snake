#version 130

uniform sampler2D fbo_texture;
uniform float random_seed;
uniform float time;

in vec2 uv;

float random(float seed)
{
	return fract(sin(dot(seed * uv.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float r = (random(random_seed) > (0.996f + sin(time * 10) * 0.004f) ? 1 : 0);
	vec2 uv_shifted = uv + vec2(cos(cos(time * 5) * 10) / 100, 0);
	gl_FragColor = vec4(cos(time * 2) * r, sin(time) * r, mod(time, 1) * r, 1) / 1.5f + texture(fbo_texture, uv_shifted);
	vec2 cell = mod(uv_shifted, 1.0f / 25.0f);
	if ((random(random_seed*2) >= 1 - sin(sin(time*2)*5)) && ((cell.x < 0.001f) || (cell.y < 0.001f))) gl_FragColor = vec4(0.6, 0.8, 1, 1) * random(random_seed*2);
}
