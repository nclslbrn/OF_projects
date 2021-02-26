#pragma once
#include "ofMain.h"

class MeshFrame {
   public:
    MeshFrame();
    MeshFrame(ofPixels framePixels, int threshold) {
    }
    void update();
    void draw();

    ofMesh frameMesh;
}