#include "Palette.h"
/**
 * Class to load palette from Chromotome into OF sketch
 * 
 * Chromotome created by Kjetil Midtgarden Golid @kgolid is under MIT Licence.
 * https://github.com/kgolid/chromotome
 * 
 * You need to transcode it from js to json, see bin/data/palette/convert-to-json.js
 */
Palette::Palette(string jsonFilename, int paletteId) {
    ofFile file(jsonFilename);
    ofJson paletteData;

    if (file.exists()) {
        file >> paletteData;
        if (paletteId = -1) {
            paletteId = rand() % paletteData.size();
        }
        int currentPaletteId = 0;
        for (auto& palette : paletteData) {
            if (currentPaletteId == paletteId) {
                if (!palette.empty()) {
                    this->name = palette["name"] != NULL ? palette["name"] : "untilted";
                    if (palette["colors"] != NULL) {
                        for (auto& color : palette["colors"]) {
                            this->colors.push_back(colFromWebHex(color));
                        }
                    }
                    this->stroke = palette["stroke"] != NULL ? colFromWebHex(palette["stroke"]) : ofColor(50, 50, 50);
                    this->background = palette["background"] != NULL ? colFromWebHex(palette["background"]) : ofColor(50, 50, 50);

                } else {
                    ofLog(
                        OF_LOG_ERROR,
                        "We can't parse the palette #" + ofToString(paletteId) + ".");
                }
            }
            currentPaletteId++;
        }
    }
}

ofColor Palette::colFromWebHex(string hexValue) {
    hexValue.erase(0, 1);
    return ofColor::fromHex(ofHexToInt("0x" + hexValue));
}
string Palette::getName() { return this->name; }
ofColor Palette::getColor(int colorId) { return this->colors[colorId]; }
vector<ofColor> Palette::getColors() { return this->colors; }
int Palette::getRandomColorId() {
    int colorId = floor(this->colors.size() * ofRandom(1));
    return colorId;
}
ofColor Palette::getStroke() {
    return this->stroke;
}
ofColor Palette::getBackground() { return this->background; }