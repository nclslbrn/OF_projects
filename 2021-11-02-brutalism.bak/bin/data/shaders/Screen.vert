//#version 150

in vec4 position;
in vec2 texcoord;
uniform float u_size;

// these are for the programmable pipeline system and are passed in
// by default from OpenFrameworks
uniform mat4 modelViewProjectionMatrix;

// this is something we're creating for this shader
out vec2 texCoordVarying;

void main()
{
    // here we move the texture coordinates
    texCoordVarying=vec2(texcoord.x,texcoord.y);
    
    // send the vertices to the fragment shader
    // texCoordVarying=texcoord;
    gl_Position=modelViewProjectionMatrix*position;
    gl_PointSize=u_size;
}