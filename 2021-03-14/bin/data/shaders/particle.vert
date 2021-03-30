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
uniform float u_layer;
//uniform vec2 u_mouse;
//uniform vec3 u_camera;
//uniform vec2 u_frameRes;
uniform vec2 u_screenRes;
uniform float u_scale;
uniform samplerBuffer u_frameTex;
uniform sampler2D spark;
//uniform vec2 u_repulsor;

out vec4 color;

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
    float noiseFrequency=500;
    float noiseScaling=1000;
    float radius=2000.;
    
    vec4 pos=modelViewProjectionMatrix*transformMatrix*position;
    float n=noise((pos.xy+u_time)/noiseFrequency);
    /* float distRep=distance(pos.xy,vec2(0.));
    if(pos.y>.5&&u_layer>.2){
        
        vec2 dirRep=((pos.xy)-vec2(0));
        float displaceNoiseRep=noise((pos.xy+u_time)/noiseFrequency)*noiseScaling;
        float distNormRep=distRep/radius;
        
        distNormRep=1.-distNormRep;
        dirRep*=distNormRep;
        
        vec2 displacementRep=vec2(
            cos(displaceNoiseRep*PI)*u_layer*distNormRep,
            sin(displaceNoiseRep*PI)*u_layer*distNormRep
        );
        pos.xy+=displacementRep.xy;
        pos.y+=u_layer*u_scale;
    } */
    
    float smoothDepth=smoothstep(.01,.6,1-u_layer);
    vec4 particleColor=mix(vec4(0.,0.,u_layer,0.),instanceColor,smoothDepth);
    pos.y-=200*u_layer*n;
    color=particleColor;
    
    gl_PointSize=n*(1.-u_layer);
    gl_Position=pos;
}