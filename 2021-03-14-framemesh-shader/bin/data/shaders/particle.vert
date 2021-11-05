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
uniform float u_noiseFreq;
uniform float u_noiseScale;

uniform vec2 u_screenRes;
uniform float u_scale;
uniform samplerBuffer u_frameTex;
uniform sampler2D spark;

out vec4 color;
out float uVsize;
out float uEdgeSize;

// 3D noise from Patricio Gonzalez Vivo
// source: https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83

float mod289(float x){return x-floor(x*(1./289.))*289.;}
vec4 mod289(vec4 x){return x-floor(x*(1./289.))*289.;}
vec4 perm(vec4 x){return mod289(((x*34.)+1.)*x);}

float noise(vec3 p){
    vec3 a=floor(p);
    vec3 d=p-a;
    d=d*d*(3.-2.*d);
    
    vec4 b=a.xxyy+vec4(0.,1.,0.,1.);
    vec4 k1=perm(b.xyxy);
    vec4 k2=perm(k1.xyxy+b.zzww);
    
    vec4 c=k2+a.zzzz;
    vec4 k3=perm(c);
    vec4 k4=perm(c+1.);
    
    vec4 o1=fract(k3*(1./41.));
    vec4 o2=fract(k4*(1./41.));
    
    vec4 o3=o2*d.z+o1*(1.-d.z);
    vec2 o4=o3.yw*d.x+o3.xz*(1.-d.x);
    
    return o4.y*d.y+o4.x*(1.-d.y);
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
    float smoothDepth=smoothstep(0.,.8,u_layer);
    float easeDepth=ease(u_layer,1.5);
    float noiseScale=easeDepth*u_noiseScale;
    
    vec2 st=normalize(modelPos.xy);
    float noisePosValue=noise(vec3(modelPos.xy,u_time)/u_noiseFreq);
    float noiseSizeValue=noise(vec3((pos.xy+u_layer)/u_noiseFreq,u_time));
    // noise displacement increase from front to back
    float noiseDisplacement=noiseScale*noisePosValue;
    vec4 newPos=pos+normal*(easeDepth*noiseDisplacement);
    
    vec4 diedColor=vec4(.0);
    vec4 particleColor=mix(instanceColor,diedColor,easeDepth);
    //vec4 posColor=vec4(1.);
    //posColor.bg=st;
    //posColor.r=noisePosValue;
    //posColor.a=u_layer*.1;
    //vec4 particleColor=mix(posColor,instanceColor,easeDepth);
    float aSize=u_layer;
    float vSize=2.*sqrt(aSize/PI);
    float edgeSize=u_time;
    uVsize=vSize;
    uEdgeSize=edgeSize;
    color=particleColor;
    
    //gl_PointSize=noiseSizeValue*2.5;
    gl_PointSize=vSize+edgeSize+1.;
    gl_Position=newPos;
}
