#include "Constant.h"

Constant::Constant(string jsonFilename, int constId) {
    ofFile file(jsonFilename);
    ofJson constantArray;

    if (file.exists()) {
        file >> constantArray;
        int currConstId = 0;
        for (auto values : constantArray) {
            if (constId == currConstId) {
                if (!values.empty()) {
                    this->A = ofToFloat(values["a"]);
                    this->B = ofToFloat(values["b"]);
                    this->C = ofToFloat(values["c"]);
                    this->D = ofToFloat(values["d"]);
                }
            }
            currConstId++;
        }
    } else {
        std::cout << jsonFilename << " doesn't exist or is not readable." << endl;
    }
}
float Constant::getA() { return this->A; }
float Constant::getB() { return this->B; }
float Constant::getC() { return this->C; }
float Constant::getD() { return this->D; }