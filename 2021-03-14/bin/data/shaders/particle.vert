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

void main(){
    
    int x=gl_InstanceID*4;
    mat4 transformMatrix=mat4(
        texelFetch(u_frameTex,x),
        texelFetch(u_frameTex,x+1),
        texelFetch(u_frameTex,x+2),
        texelFetch(u_frameTex,x+3)
    );
    vec4 modelPos = transformMatrix*position;
    vec4 pos=modelViewProjectionMatrix*modelPos;
    float smoothDepth=smoothstep(0.2,0.6,1.2-u_layer);
    float noiseFrequency=0.01;
    float noiseScale = 10.0 * smoothDepth;
    vec2 st=normalize(modelPos.xy);
    float n=noiseScale * noise((modelPos.xy + u_time)/noiseFrequency);
    
    
    vec4 newPos = pos + normal * n;
    // pos.x-=noiseScale*cos(n*PI*2.0)*(1.0-smoothDepth);
    // pos.y-=noiseScale*sin(n*PI*2.0)*(1.0-smoothDepth);
    //pos.y = (pos.y/u_screenRes.y) * n * (1.0-smoothDepth);
    
    vec4 particleColor=mix(vec4(0.15, 0.75, 0.15,0.),instanceColor,smoothDepth);
    // vec4 particleColor=vec4(0.5);
    // particleColor.rg=st;
    // particleColor.a=1.0;
    color=particleColor;
    
    gl_PointSize=2.0*n*(1.0-smoothDepth);
    gl_Position=newPos;
}
