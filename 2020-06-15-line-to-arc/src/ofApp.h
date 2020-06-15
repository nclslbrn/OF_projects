#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void init();

    float ease(float p);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void onGifSaved(string& fileName);
    void exit();

    vector<ofVec2f> positions;
    vector<float> distances;
    vector<float> thetas;
    float angle;
    float initRadius;
    float radius;
    int numArcs = 1280;
    float circleRes = 0.15;
    float margin = 64;
    bool isInitialized = false;
    bool isDebugActive = false;
    // Gif animation / export setup
    ofxGifEncoder gifEncoder;
    int numFrames{30};
    int currFrame = 0;
    ofImage img;
    bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
