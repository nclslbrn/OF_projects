//#version 150

in vec4 modelPosition;
in vec4 color;
in vec2 texcoord;
in float uVsize;
in float uEdgeSize;

out vec4 outColor;


void main(){
	float dist = length(2.0 * gl_PointCoord - 1.0);
	if (dist > 1.0) {
		discard;
	}
	float sEdge = smoothstep(
		uVsize - uEdgeSize - 2.0,
		uVsize - uEdgeSize,
		dist * (uVsize + uEdgeSize)
	);
	outColor = (color * sEdge) + ((1.0 - sEdge) * gl_FragColor);
}