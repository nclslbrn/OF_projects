#pragma once
#include "ofMain.h"

class MeshFrame {
   public:
    MeshFrame();
    MeshFrame(ofPixels framePixels) {
    }
    void update();
    void draw();

    ofMesh frameMesh;
}