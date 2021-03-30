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
uniform float u_layer;
uniform vec2 u_mouse;
uniform vec3 u_camera;
uniform vec2 u_frameRes;
uniform vec2 u_screenRes;
uniform float u_scale;
uniform samplerBuffer u_frameTex;
uniform sampler2D spark;
uniform vec2 u_repulsor;

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
    float noiseFrequency=1.;
    float noiseScaling=1000;
    float radius=2000.;
    
    vec4 pos=transformMatrix*position;
    /* random pos from u_repulsor */
    float distRep=distance(pos.xy,u_repulsor.xy);
    if(distRep>0&&distRep<radius){
        vec2 dirRep=((pos.xy)-u_repulsor);
        float displaceNoiseRep=noise((pos.xy+u_time)/noiseFrequency)*noiseScaling;
        float distNormRep=distRep/radius;
        
        distNormRep=1.-distNormRep;
        dirRep*=distNormRep;
        
        vec2 displacementRep=vec2(
            cos(displaceNoiseRep*PI)*u_layer*distNormRep,
            sin(displaceNoiseRep*PI)*u_layer*distNormRep
        );
        pos.xy+=displacementRep.xy;
        pos.y+=u_layer*200*u_scale;
    }
    float dist=distance(position.xy,vec2(1,0));
    float distNorm=dist/radius;
    distNorm=1.-distNorm;
    float displaceNoise=noise((pos.xy*u_scale*u_layer)/noiseFrequency)*noiseScaling;
    vec2 displacement=vec2(
        cos(displaceNoise)*distNorm,
        sin(displaceNoise)*distNorm
    );
    if(dist>.5&&dist<1.){
        pos.zw*=vec2(u_layer*.01);
        pos.xy*=displacement*mix(1.,8000*u_time,1.-u_layer);
        
    }
    // zoom in
    //pos.xyz*=vec3(u_layer*mix(1.,3.,u_time));
    //pos.xy+=mix(mix(1.,3.,u_time),2000*u_time,1.-step(.5,u_layer));
    
    //vec4 particleColor=instanceColor;
    //vec4 particleAlpha=texture2D(spark,texcoord);
    //particleColor.a=particleAlpha.a;
    //particleColor.rgb+=(.5-u_layer);
    //particleColor.a-=smoothstep(.25,.75,u_layer);
    //color=particleColor;
    float smoothDepth=smoothstep(.2,.95,u_layer);
    vec4 particleColor=mix(vec4(0),instanceColor,smoothDepth);
    particleColor.a=smoothDepth;
    color=particleColor;
    
    // gl_PointSize=(1-u_layer)*.5;
    gl_Position=modelViewProjectionMatrix*pos;
}