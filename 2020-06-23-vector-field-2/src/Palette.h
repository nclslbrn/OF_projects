#pragma once
#include "ofMain.h"

class Palette {
   public:
    string name;
    vector<ofColor> colors;
    ofColor stroke;
    ofColor background;

    Palette(string jsonFilename, int paletteId = -1);
    ofColor colFromWebHex(string hexValue);
    string getName();
    ofColor getColor(int colorId);
    vector<ofColor> getColors();
    int getRandomColorId();
    ofColor getStroke();
    ofColor getBackground();
};