// #version 150

in vec4 colorVarying;
uniform float u_size;
out vec4 outputColor;

void main (void) {
	outputColor = colorVarying; // * vec4(1.0, 1.0, 1.0, 0.8 + u_size);
}
