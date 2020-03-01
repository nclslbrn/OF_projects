#pragma once

#include "ofMain.h"
#include "Point.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void reset();
    void saveLargeImage(string imgName);
    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);

    ofColor createHue( int n);
    //ofxJSONElement dbFile;
    // Attractor constants
    double A, B, C, D;
    ofVec2f position;
    deque<Point> points;
    float maxSize, minSize;
    // how displacement on pos, displacement counter
    int iterations, steps, i, width, height;
    // expand coordinate (polar system)
    float minX, minY, maxX, maxY;
    // our canvas
    ofFbo fbo;
    std::string imgName;
};
