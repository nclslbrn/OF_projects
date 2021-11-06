// #version 150

uniform sampler2DRect u_crop_tex;
uniform vec2 u_crop_res;
uniform float u_alpha;

in vec4 colorVarying;

out vec4 outputColor;

void main (void) {
	vec4 texColor = texture(u_crop_tex, gl_PointCoord.xy);
	outputColor= colorVarying; // vec4(1.0, 1.0, 1.0, u_alpha);
	//outputColor = texture(tex, gl_PointCoord) * colorVarying;
}
