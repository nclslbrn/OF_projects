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
uniform vec2 u_screenRes;
uniform float u_scale;
uniform samplerBuffer u_frameTex;
uniform sampler2D spark;

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

// beesandbombs ease function
float ease(float p,float g){
    if(g!=0){
        if(p<.5){
            return.5*pow(2*p,g);
        }else{
            return 1-.5*pow(2*(1-p),g);
        }
    }else{
        return 3*p*p-2*p*p*p;
    }
}

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
    
    // u_layer 0 ot 1 from first to last frame
    float smoothDepth=smoothstep(.2,1.,u_layer);
    float easeDepth=ease(u_layer,.5);
    float noiseFrequency=10.;
    float noiseScale=4000.*easeDepth;
    
    vec2 st=normalize(modelPos.xy);
    float noisePosValue=noise((modelPos.xy+u_time)/noiseFrequency);
    float noiseSizeValue=noise((pos.xy+u_layer)/noiseFrequency);
    // noise displacement increase from front to back
    float noiseDisplacement=noiseScale*noisePosValue;
    
    vec4 newPos=pos+normal*(easeDepth*noiseDisplacement);
    
    vec4 diedColor=vec4(.3,.3,.3,0);
    vec4 particleColor=mix(instanceColor,diedColor,easeDepth);
    // vec4 posColor=vec4(1.);
    // posColor.rg=st;
    // vec4 particleColor=mix(posColor,diedColor,easeDepth);
    
    color=particleColor;
    
    //gl_PointSize=noiseSizeValue*2.5;
    gl_PointSize=noiseSizeValue*.1;
    gl_Position=newPos;
}
