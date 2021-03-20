#pragma once
#include "FrameMesh.h"
#include "MouseHeatMap.h"
#include "ofMain.h"
#include "ofxShaderFilter.h"

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
    ofVec2f getRandomPos(ofVec2f c, float scale);

    ofVec2f center = ofVec2f(ofGetWidth() / 2.0, ofGetHeight() / 2.0);
    vector<ofVec2f> repulsor;

    int numFrame = 600;
    float meshScale = 12;
    MouseHeatMap heatMap;
    FrameMesh frame;
    ofxShader shader;
    ofEasyCam camera;
};
