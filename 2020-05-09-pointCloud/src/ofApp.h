#pragma once

#include "ofMain.h"
/* https://github.com/braitsch/ofxDatGui */
#include "ofxDatGui.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();

    ofxDatGui* gui;

    void initDatGui();
    void debugAxis();
    void cameraMove();
    void keyPressed(int key);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);

    int animFrame;

    bool debug;
    ofParameter<float> cameraXAngle, cameraXPos;
    ofParameter<int> cameraYStart, cameraYEnd;
    ofParameter<float> displaceRadius, noiseScale;
    ofVec3f camStartPos, camTargetPos;
    ofSpherePrimitive camCollider;

    ofMesh mesh;
    ofCamera camera;
    ofEasyCam debugCam;
    ofShader shader;
};
