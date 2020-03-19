#pragma once

#include "ofMain.h"

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
    void setNormals(ofMesh &mesh);
    float ease(float p);
    float softPlus(float q, float p);

    ofLight pointLight;
    ofMesh mainMesh;
    ofMaterial material;
    ofEasyCam mainCam;

    float noiseScale, noiseRadius;
    int animFrame, width, height, depth;
};
