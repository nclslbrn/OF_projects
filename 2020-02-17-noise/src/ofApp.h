#pragma once

#include "ofMain.h"

class Line {
   public:
    ofPoint a;
    ofPoint b;
};

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

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
    float ease(float p);
    float softPlus(float q, float p);

    int animFrame;

    float noiseScale;
    float radius;
    int width;
    int height;
    int stepX;
    int stepY;

    ofPolyline line;
    vector<ofPoint> drawnPoints;
    vector<Line> lines;
    vector<Line> noisedLines;

    ofEasyCam easyCam;
};
