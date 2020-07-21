#pragma once

#include "Palette.h"
#include "ofMain.h"

/**
 * PLANAR FUNCTION STUDY
 * 
 * based on works of Tsulej
 * https://generateme.wordpress.com/2016/04/24/drawing-vector-field/ 
 */

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mousePressed(int x, int y, int button);

    // Available/converted paletteGroup of Chomotome (JSON FILES)
    // judson, misc, rohlfs, system, colourscafe, dale, iivonen, ducci
    Palette palette = Palette("palette/json/judson.json");

   private:
    float ease(float p);
    ofVec2f circle(float n);
    ofVec2f astroid(ofVec2f v);
    ofVec2f quadrifolium(ofVec2f v);
    ofVec2f rect_hyperbola(ofVec2f v);
    ofVec2f trifolium(ofVec2f v);
    ofVec2f cardioid(ofVec2f v);
    ofVec2f deltoid(ofVec2f v);
    ofVec2f ranunculoid(ofVec2f v);
    ofVec2f cycloid(ofVec2f v);

    vector<ofVec2f> pos;
    vector<int> colors;
    int width = 1080;
    int height = 1080;
    float vectorScale = 0.1f;
    float density = 0.07;
    int numFrame = 60;
    int alpha = 5;
    ofFbo cache;
};
