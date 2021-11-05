//#version 150

in vec2 texCoordVarying;

// this is how we receive the texture
uniform sampler2DRect tex0;

out vec4 outputColor;

void main()
{
    outputColor=texture(tex0,texCoordVarying);
}