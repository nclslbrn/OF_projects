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
    ofImage source;
    ofImage modified;

    int currY;
    int width, height;
    int pixStep = 1;
    int brightThreshold = 127;
    int darkThreshold = 127;
    int brightnessVariation = 150;
    bool isSaved = false;
    string sourceName = "usgs-R_W86FHa-Sk-unsplash.jpg";
    string sourceSize = "1280x1280";  // "1080x1080" "800x800"  "400x400"
};
