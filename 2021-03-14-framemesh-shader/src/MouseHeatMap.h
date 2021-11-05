#pragma once
#include "ofMain.h"

class MouseHeatMap {
   public:
    MouseHeatMap();
    MouseHeatMap(int width, int height, float initRadius);
    void draw(int x, int y, int width, int height);
    void capture(int x, int y);
    void bind();
    void unbind();
    ofFbo getFbo();
    ofTexture getMaptexture();
    float radius;
    ofFbo map;
};