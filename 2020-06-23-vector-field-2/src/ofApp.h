#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);

   private:
    float ease(float p);
    ofVec2f circle(float n);
    ofVec2f astroid(float n);
    ofVec2f quadrifolium(float n);
    ofVec2f rect_hyperbola(float n);
    ofVec2f trifolium(float n);
    ofVec2f cardioid(float n);
    ofVec2f deltoid(float n);

    vector<ofVec2f> pos;
    vector<int> colors;
    int width = ofGetWidth();
    int height = ofGetHeight();
    float vectorScale = 0.01f;
    float canvasScale = 0.25;
    float density = 0.07;
    int numFrame = 60;

    vector<ofColor> palette = {
        ofColor(0, 91, 197),
        ofColor(0, 180, 252),
        ofColor(23, 249, 255),
        ofColor(223, 147, 0),
        ofColor(248, 190, 0)};

    ofFbo cache;
};
