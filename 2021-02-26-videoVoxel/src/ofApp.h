#pragma once

#include "MeshFrame.h"
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

    // video player object
    ofVideoPlayer videoSample;
    // frames buffer
    deque<ofPixels> frames;
    // frames buffer size, pixel jump
    int nFrame, res;

    // pixels array for constructing output image
    ofPixels imagePixels;
    // output image
    ofImage image;

    // voxels array
    deque<MeshFrame> meshFrames;

    ofEasyCam cam;
    ofMesh mesh;
    ofImage img;
};
