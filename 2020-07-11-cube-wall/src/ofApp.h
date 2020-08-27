#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void onGifSaved(string& fileName);
    void animation(float t, float alpha, int n);
    void exit();
    void cube(float d, float alpha);
    float ease(float p);

    // Motion blur
    ofFbo cache;
    int samplesPerFrame = 6;
    float shutterAngle = 0.8f;

    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames = 60;
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;

    // Animation variable
    int N = 8;
    int w, h;
    ofLight light;
    ofCamera cam;
    vector<ofColor> palette = {
        ofColor(46, 41, 78),
        ofColor(231, 29, 54),
        ofColor(244, 96, 54),
        ofColor(27, 153, 139),
        ofColor(197, 216, 109)};
    /*
    vector<ofColor> palette = {
        ofColor(255, 159, 28),
        ofColor(247, 23, 53),
        ofColor(253, 255, 252),
        ofColor(65, 234, 212),
        ofColor(1, 22, 39)

    };
    */
};
