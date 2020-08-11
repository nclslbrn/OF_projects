#pragma once

#include "Constant.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void searchNewConstant();
    void saveLargeImage(string imgName);
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    bool findNewConstants = false;
    // Attractor constants
    int constantsNum = 0;
    string jsonFilename = ofToDataPath("constant/done.json");
    Constant constant = Constant(jsonFilename, constantsNum);
    void nextConstant();
    double A, B, C, D;
    ofVec2f position;
    // how displacement on pos, displacement counter
    int iterations, steps, i, width, height;
    // expand coordinate (polar system)
    float minX, minY, maxX, maxY;
    // our canvas
    ofFbo fbo;
    std::string imgName;
};
