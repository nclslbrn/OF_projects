//#version 150

#define PI 3.1415926538

uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 instanceColor;
in vec4 normal;
in vec2 texcoord;

uniform float u_time;
uniform vec2 u_mouse;
uniform vec3 u_camera;
uniform vec2 u_frameRes;
uniform samplerBuffer u_frameTex;
uniform sampler2D tex;
uniform vec2 u_repulsor;

out vec4 color;
out vec2 texCoordVarying;

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
    /* make radius increase and decrease
    if(u_time<.5){
        radius=(u_time+u_time)*8000;
    }else{
        radius=(2-(u_time+u_time))*8000;
    } */
    
    
    vec4 newPos=position;
    vec4 pos=transformMatrix*newPos;
    float posNoise=noise(pos.xy);
    
    
    /* random pos from u_repulsor */
    vec2 rep= u_repulsor;
    vec2 dir=((pos.xy)-rep);
    float brushSize = 512.0;
    vec2 noisedRep = vec2(rep.x + (cos(posNoise * PI*2.0) * brushSize), rep.y + (sin(posNoise * PI*2.0) * brushSize));
    float dist=distance(pos.xy, noisedRep);
    if(dist>0&&dist<radius){
        float displaceNoise=noise((pos.xy+u_time)/noiseFrequency)*noiseScaling;
        float distNorm=dist/radius;
        
        distNorm=1.-distNorm;
        dir*=distNorm;
        
        vec2 displacement=vec2(
            cos(displaceNoise*PI*2.0)*400*distNorm,
            sin(displaceNoise*PI*2.0)*400*distNorm
        );
        pos.xy+=displacement.xy;
        
        color = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        color=instanceColor;
        
    }
    
    /** distortion with heatmap
    vec4 vTexCoord = textureMatrix*vec4(texcoord.x,texcoord.y,0,1);
    vec4 hotCol = texture(tex, texcoord);
    
    float variation = smoothstep(0.0, 1.0, hotCol.r) * 50;
    vec2 displacement=vec2(
        cos(noise)*variation,
        sin(noise)*variation
    );
    pos.xy+=displacement.xy;
    if( hotCol.r > 0.05 ) {
        color = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        color=instanceColor;
    }
    */
    
    /* make particles fade out at loop end
    float fade=smoothstep(0.,1.,u_time);
    color.a-=fade;
    */
    gl_Position=modelViewProjectionMatrix*pos;
    texCoordVarying=texcoord;
    
}
