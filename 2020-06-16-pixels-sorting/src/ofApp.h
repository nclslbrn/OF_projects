#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void addLine(
        int x,
        int y,
        ofColor pixColor,
        int brightness,
        bool searchBrightPix);
    void exit();

   private:
    int nextBrightY(int x, int y, int brightness);
    int nextDarkY(int x, int y, int brightness);
    ofImage source;
    ofImage modified;
    bool isSaved = false;

    int currY;
    int width, height;
    int pixStep = 2;
    int brightThreshold = 180;
    int darkThreshold = 70;
    int queryLigthThreshold = 50;
    int brightnessVariation = 100;
    string sourceName = "3409485258_7b735fcedb_k.jpg";
    string sourceSize = "800x800";  //  "1080x1080" "800x800"
};
