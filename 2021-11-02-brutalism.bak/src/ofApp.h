#pragma once

#include "ofMain.h"
#include "ofVbo.h"
#include "ofxShaderFilter.h"

#define NUM_BILLBOARDS 1000

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void nextMove();
    void draw();

    bool goForward;
    bool isVertical;
    ofRectangle rect;
    float stepSize;
    float size;
    int d;
    int numFrame;

    ofVec2f center;
    ofImage sample;
    ofImage sampleCroped;
    ofPixels crop;
    ofxShader screenShader;
    ofxShader billboardShader;

    ofPlanePrimitive screen;

    ofEasyCam cam;

    // of/exmaples/billboardExemple
    glm::vec3 cameraRotation;
    float zoom, zoomTarget;

    // billboard particles
    float billboardSizeTarget[NUM_BILLBOARDS];
    ofVboMesh billboards;
    glm::vec3 billboardVels[NUM_BILLBOARDS];
};