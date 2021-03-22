//#version 150

uniform sampler2DRect u_sparkTex;

in vec4 color;
in vec2 texcoord;

out vec4 outColor;

void main(){
	float border = 0.3;
	float radius = 0.8;	
	float dist = radius - distance(vec2(0.1), vec2(0.5));
	float t = smoothstep(0.0, border, dist);
	
	vec4 alphaColor = color;
	alphaColor.a = t;
	outColor = alphaColor;
}