//#version 150
#define PI 3.1415926538
//#extension GL_EXT_gpu_shader4:enable
//#extension GL_ARB_texture_rectangle:enable

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 instanceColor;
in float point_size;
in vec4 normal;
in vec2 texcoord;

uniform float u_time;
uniform samplerBuffer u_frameTex;
uniform sampler2D u_sparkTex;

out vec4 color;
out vec4 particlePos;
out float uVsize;
out float uEdgeSize;

void main(){
    
    int x=gl_InstanceID*4;
    mat4 transformMatrix=mat4(
        texelFetch(u_frameTex,x),
        texelFetch(u_frameTex,x+1),
        texelFetch(u_frameTex,x+2),
        texelFetch(u_frameTex,x+3)
    );
    vec4 modelPos=transformMatrix*position;
    vec4 pos=modelViewProjectionMatrix*modelPos;
    
    float aSize=u_time*2;
    float vSize=2.*sqrt(aSize/PI);
    float edgeSize=u_time;
    
    uVsize=vSize;
    particlePos=pos;
    color=instanceColor;
    uEdgeSize=edgeSize;
    
    gl_PointSize=vSize+edgeSize+1.;
    gl_Position=pos;
}
