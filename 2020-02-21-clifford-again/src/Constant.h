#pragma once
#include "ofMain.h"

class Constant {
   public:
    float A;
    float B;
    float C;
    float D;
    Constant(string jsonFilename, int constId);
    float getA();
    float getB();
    float getC();
    float getD();
};