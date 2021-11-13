#pragma once
#include "ofMain.h"

class Layer {
   public:
    Layer(
        int cols,
        int rows,
        int cellSize,
        float noiseDepth,
        int z);

    void update(float noiseDepth = -1.0f);

    int getZpos();

    vector<ofVec2f> getPoints(
        float threshold,
        int pointX = -1,
        int pointY = -1);

   private:
    void computeNoise();
    vector<ofVec2f> getNearPoints(ofVec2f target);
    vector<ofVec2f> points;
    vector<float> weights;
    int cols, rows;
    int noiseDepth, z;
};
