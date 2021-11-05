//#version 150
in vec4 position;
in vec4 color;
in vec3 normal;

uniform mat4 orientationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec4 colorVarying;

void main()
{
    vec4 eyeCoord=modelViewMatrix*position;
    // to be accurate this should premultiply by orientationMatrix
    gl_Position=projectionMatrix*eyeCoord;
    
    float dist=sqrt(eyeCoord.x*eyeCoord.x+eyeCoord.y*eyeCoord.y+eyeCoord.z*eyeCoord.z);
    float att=50./dist;
    
    gl_PointSize=normal.x*att;
    colorVarying=color;
}

