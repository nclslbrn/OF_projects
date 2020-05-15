// fragment shader

#version 150

out vec4 outputColor;
uniform float time;
uniform vec4 cameraPos;
uniform vec2 resolution;
uniform float bloomIntensity;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    // gradient left/right blue/pink
    float r = gl_FragCoord.x / resolution.x;
    float g = gl_FragCoord.y / resolution.y;
    float m = 1.0;

    float centerX = resolution.x/2;
    if( gl_FragCoord.x < centerX ) {
        m = map( gl_FragCoord.x, 0, centerX, 0.5, 1);
    } else {
        m = map( gl_FragCoord.x, centerX, resolution.x, 1, 0.5);;
    }
    outputColor = vec4(r, g, 1.0, m);

}