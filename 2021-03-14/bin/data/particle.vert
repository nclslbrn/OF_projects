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
in vec4 normal;
in vec2 texcoord;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec3 u_camera;
uniform vec2 u_frameRes;
uniform vec2 u_screenRes;
uniform float u_scale;
uniform samplerBuffer u_frameTex;
uniform sampler2D spark;
uniform vec2 u_repulsor;

out vec4 color;
out vec2 texCoordVarying;
out vec4 modelPosition;

float random(in vec2 st){
    return fract(
        sin(
            dot(st.xy,vec2(12.9898,78.233))
        )*43758.5453123
    );
}

float noise(in vec2 st){
    vec2 i=floor(st);
    vec2 f=fract(st);
    
    // Four corners in 2D of a tile
    float a=random(i);
    float b=random(i+vec2(1.,0.));
    float c=random(i+vec2(0.,1.));
    float d=random(i+vec2(1.,1.));
    
    // Smooth Interpolation
    
    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u=f*f*(3.-2.*f);
    // u = smoothstep(0.,1.,f);
    
    // Mix 4 coorners percentages
    return mix(a,b,u.x)+
    (c-a)*u.y*(1.-u.x)+
    (d-b)*u.x*u.y;
}

void main(){
    
    int x=gl_InstanceID*4;
    mat4 transformMatrix=mat4(
        texelFetch(u_frameTex,x),
        texelFetch(u_frameTex,x+1),
        texelFetch(u_frameTex,x+2),
        texelFetch(u_frameTex,x+3)
    );
    float noiseFrequency=.5;
    float noiseScaling=3;
    float radius=2000.;
    
    vec4 pos=transformMatrix*position;
    
    /* random pos from u_repulsor */
    float dist=distance(pos.xy,u_mouse);
    if(dist>0&&dist<radius){
        vec2 dir=((pos.xy)-u_mouse);
        float displaceNoise=noise((pos.xy+u_time)/noiseFrequency)*noiseScaling;
        float distNorm=dist/radius;
        
        distNorm=1.-distNorm;
        dir*=distNorm;
        
        vec2 displacement=vec2(
            cos(displaceNoise*PI*2.)*100*distNorm,
            sin(displaceNoise*PI*2.)*100*distNorm
        );
        pos.xy+=displacement.xy;
    }
    /*
    vec4 eyeCoord=modelViewProjectionMatrix*pos;
    float distB=sqrt(eyeCoord.x*eyeCoord.x+eyeCoord.y*eyeCoord.y+eyeCoord.z*eyeCoord.z);
    float att=50./distB;
    */
    vec4 particleColor=instanceColor;
    vec4 particleAlpha=texture2D(spark,texcoord);
    particleColor.a=particleAlpha.a;
    color=particleColor;
    // color=vec4(texcoord,0.,1.);
    
    //gl_PointSize=normal.x*att;
    gl_Position=modelViewProjectionMatrix*pos;
    texCoordVarying=texcoord;
}