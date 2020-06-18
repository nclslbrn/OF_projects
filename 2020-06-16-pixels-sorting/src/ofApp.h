#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void addLine(int x, int y, ofColor pixColor, int brightness, bool searchBrightPix);
    void exit();
    int nextBrightY(int x, int y);
    int nextDarkY(int x, int y);
    ofImage picture;
    int currY;
    int lineLenght = 64;
    int width, height;
    int brightThreshold = 175;
    int darkThreshold = 175;
    int brightnessVariation = 50;
    bool isSaved = false;
    string sourceImage = "nicolas-lebrun-Melbourne-1.jpg";
};
