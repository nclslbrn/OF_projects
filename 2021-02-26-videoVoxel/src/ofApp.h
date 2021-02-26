#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    // video player object
    ofVideoPlayer videoSample;
    // frames buffer
    deque<ofPixels> frames;
    // frames buffer size, pixel jump
    int nFrame, res;
    int threshold = 34;
    // pixels array for constructing output image
    ofPixels imagePixels;
    // output image
    ofImage image;

    // voxels array
    int numVisible;
    ofShader shader;
    ofEasyCam cam;
    ofMesh mesh;
    ofImage img;

    // shader uniform
    float uTime = 0;
    float uRandom = 0;
    float uDepth = 0;
    float uSize = 0;
    ofVec2f uTextureSize;
    
};
