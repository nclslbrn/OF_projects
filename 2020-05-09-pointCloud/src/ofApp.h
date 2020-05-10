#pragma once

#include "ofMain.h"
/* https://github.com/AmnonOwed/ofxPointInMesh */
#include "ofxPointInMesh.h"
/* https://github.com/braitsch/ofxDatGui */
#include "ofxDatGui.h"

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

    void initDatGui();
    void debugAxis();
    void cameraMove();
    void pointCloudErode();
    void onSliderEvent(ofxDatGuiSliderEvent e);

    ofxDatGui* gui;

    int animFrame, cameraColliderSize;
    bool debug;
    ofParameter<float> cameraXAngle, cameraXPos;
    ofVec3f camStartPos, camTargetPos;
    ofBoxPrimitive camNode;

    ofMesh cameraCollider, streetModel;
    ofLight point;
    ofCamera camera;
    ofEasyCam debugCam;
    ofShader shader;
};
