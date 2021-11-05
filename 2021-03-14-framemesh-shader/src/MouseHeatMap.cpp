#include "MouseHeatMap.h"

MouseHeatMap::MouseHeatMap() {
    map.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    map.setUseTexture(false);
    map.begin();
    ofClear(0, 0, 0, 255);
    map.end();
    radius = 10.0;
}
MouseHeatMap::MouseHeatMap(int width, int height, float initRadius) {
    map.allocate(width, height, GL_RGBA);
    //map.setUseTexture(false);
    map.begin();
    ofClear(0, 0, 0, 255);
    map.end();
    radius = initRadius;
}
void MouseHeatMap::draw(int x = 0, int y = 0, int width = ofGetWidth(), int height = ofGetHeight()) {
    //map.getTexture().disableAlphaMask();
    map.draw(x, y, width, height);
}

void MouseHeatMap::capture(int x = -1, int y = -1) {
    if (map.isAllocated()) {
        ofVec2f pos = ofVec2f(
            (x / float(ofGetWidth())) * map.getWidth(),
            (y / float(ofGetHeight())) * map.getHeight());

        map.begin();
        ofSetColor(0, 0, 0, 15);
        ofDrawRectangle(0, 0, map.getWidth(), map.getHeight());

        ofSetColor(255, 255, 255, 150);
        for (int i = 0; i < 10; i++) {
            ofDrawCircle(pos.x, pos.y, radius * (i / 8.0f));
        }
        map.end();
    }
}
void MouseHeatMap::bind() {
    map.getTextureReference().bind();
}
void MouseHeatMap::unbind() {
    map.getTextureReference().unbind();
}
ofFbo MouseHeatMap::getFbo() { return map; }
ofTexture MouseHeatMap::getMaptexture() { return map.getTexture(); }
