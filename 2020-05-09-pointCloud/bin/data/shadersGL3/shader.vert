
// vertex shader

#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
uniform float time;
uniform vec4 cameraPos;
uniform float displaceRadius;

void main(){

    // copy position so we can work with it.
    vec4 pos = position;

     // direction vector from camera position to vertex position.
    vec2 dir = pos.xy - cameraPos.xy;

    // distance between the camera position and vertex position.
    float dist =  distance(pos.xy, cameraPos.xy);

    if( dist > 0.0 && dist < displaceRadius) {

        // normalise distance between 0 and 1.
        float distNorm = dist / displaceRadius;

        // flip it so the closer we are the greater the repulsion.
        distNorm = 1.0 - distNorm;

        // make the direction vector magnitude fade out the further it gets from mouse position.
        dir *= distNorm;

        // add the direction vector to the vertex position.
        pos.x += dir.x;
        pos.y += dir.y;

    }

    gl_Position = modelViewProjectionMatrix * pos;
}