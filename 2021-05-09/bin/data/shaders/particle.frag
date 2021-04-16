//#version 150
in vec4 color;
in vec4 particlePos;
uniform vec2 u_resolution;
in vec4 gl_FragCoord;
out vec4 outColor;


float circleShape(vec2 position, float radius) {
	return step(radius, length(position));
}

void main(){
	vec2 position = (gl_FragCoord.xy / u_resolution) * particlePos.xy;
	vec4 circleColor = color;
	float circle = circleShape(position, 0.9);
	circleColor *= vec4(circle);
	outColor = circleColor;
}