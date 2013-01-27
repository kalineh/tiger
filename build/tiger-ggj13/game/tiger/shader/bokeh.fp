#version 120

varying vec2 uv;
varying float alpha;

uniform vec4 Color;

void main(void)
{
	float alphaFallOff = smoothstep( 0.0f, 0.85f, 1.0f - length(uv));
	float alphaFinal = alphaFallOff*alpha;
	
	vec4 color = Color;
	color.w *= alphaFinal;
	gl_FragColor = color;
	
}