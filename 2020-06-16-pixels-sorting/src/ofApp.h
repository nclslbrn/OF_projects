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

   private:
    int nextBrightY(int x, int y, int brightness);
    int nextDarkY(int x, int y, int brightness);
    ofImage source;
    ofImage modified;

    int currY;
    int width, height;
    int pixStep = 1;
    int brightThreshold = 130;
    int darkThreshold = 130;
    int queryLigthThreshold = 150;
    int brightnessVariation = 90;
    bool isSaved = false;
    string sourceName = "wolfgang-hasselmann-uDWerexH6RI-unsplash.jpg";
    string sourceSize = "800x800";  // "1280x1280" "1080x1080" "800x800"  "400x400"
};
