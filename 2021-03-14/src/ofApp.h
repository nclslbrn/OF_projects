#pragma once
#include "FrameMesh.h"
#include "ofMain.h"
#include "ofxShaderFilter.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);

    ofVec2f center = ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0);

    int framesInLoop = 12,
        minPixelsBrightness = 70;

    float meshScale = 12.0;
    ofVideoPlayer video;
    vector<FrameMesh> frames;
    ofxShader shader;
    ofEasyCam camera;
    ofImage sparkTexture;
    ofVec2f sparkTexCoord;

    ofFbo frameCapture;
    ofImage toSave;
    bool isRecording = false;
    int recordFrameNum = 0;
};
