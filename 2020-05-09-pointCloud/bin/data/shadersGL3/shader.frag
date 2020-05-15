// fragment shader

#version 150

out vec4 outputColor;
uniform float time;
uniform vec4 cameraPos;
uniform vec2 resolution;
uniform float bloomIntensity;

void main()
{
    // gradient left/right blue/pink
    float r = gl_FragCoord.x / resolution.x;
    float g = gl_FragCoord.y / resolution.y;
    float m = 1.0;

    float centerX = resolution.x/2;
    if( gl_FragCoord.x < centerX ) {
        m = (gl_FragCoord.x / centerX )/2;
    } else {
        m = 0.5 - ((gl_FragCoord.x-centerX) / centerX)/2;
    }
    outputColor = vec4(r, g, m, m);

}