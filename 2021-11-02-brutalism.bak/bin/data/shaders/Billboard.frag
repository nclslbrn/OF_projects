// #version 150

uniform sampler2DRect u_crop_tex;
uniform vec2 u_crop_res;

in vec4 colorVarying;


out vec4 outputColor;

void main (void) {
	outputColor=texture(u_crop_tex,gl_FragCoord.xy);
	//outputColor = texture(tex, gl_PointCoord) * colorVarying;
}
