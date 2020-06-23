#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void onGifSaved(string& fileName);
    void animation(float t, float alpha);
    void exit();
    float ease(float p);
    ofVec2f circle(float n);
    ofVec2f astroid(float n);
    ofVec2f quadrifolium(float n);
    ofVec2f rect_hyperbola(float n);
    ofVec2f trifolium(float n);

    vector<ofVec2f> pos;
    vector<int> colors;
    int width = ofGetWidth();
    int height = ofGetHeight();
    float vectorScale = 0.1f;
    float density = 0.03;  // less is more
    vector<ofColor> palette = {
        ofColor(0, 91, 197),
        ofColor(0, 180, 252),
        ofColor(23, 249, 255),
        ofColor(223, 147, 0),
        ofColor(248, 190, 0)};
    ofTrueTypeFont font;

    // Motion blur
    ofFbo cache;
    int samplesPerFrame = 1;
    float shutterAngle = 4.0f;

    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames{30};
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
