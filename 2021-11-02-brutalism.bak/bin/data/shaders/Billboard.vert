//#version 150
in vec4 position;
in vec4 color;
in vec3 normal;

uniform float u_size;
uniform mat4 orientationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec4 colorVarying;

void main()
{
    float att= 0.15*u_size;
    
    gl_Position = modelViewProjectionMatrix*position;
    gl_PointSize=normal.x*att;
    colorVarying=color;
}

