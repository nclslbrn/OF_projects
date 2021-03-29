#pragma once
#include "FrameMesh.h"
#include "LiveFrameReader.h"
#include "ofMain.h"
#include "ofxShaderFilter.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void exit();
    ofVec2f getRandomPos(ofVec2f c, float scale);
    ofVec2f center = ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
    vector<ofVec2f> repulsor;

    int framesInLoop = 8,
        minPixelsBrightness = 30,
        numFrame = 600;

    float meshScale = 12.0;
    ofVideoPlayer video;
    vector<FrameMesh> frames;
    LiveFrameReader nextFrame;
    ofxShader shader;
    ofEasyCam camera;
    ofImage sparkTexture;
    ofVec2f sparkTexCoord;
};
