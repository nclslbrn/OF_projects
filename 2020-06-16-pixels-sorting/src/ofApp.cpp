#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    source.load("brutalism-Buenos-Aeres.JPG");
    width = source.getWidth();
    height = source.getHeight();
    //source.allocate(width, height, OF_IMAGE_COLOR);
    //source.setColor(ofColor(125, 125, 125));
    currY = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    bool findSomething = false;
    int y = currY;
    while (!findSomething) {
        for (int x = 0; x < width; x++) {
            ofColor c = source.getColor(x, y);
            float brightness = c.getBrightness();
            if (brightness > 255 - threshold || brightness < threshold) {
                int dy;
                if (brightness < threshold) {
                    addLine(x, y, y + lineLenght, c, brightness, true);
                } else {
                    addLine(x, y - lineLenght, y, c, brightness, false);
                }
                findSomething = true;
            }
        }
        y++;
    }
    currY = y;
}
//--------------------------------------------------------------

void ofApp::addLine(int x, int fromY, int toY, ofColor pixColor, int brightness, bool goDown) {
    int b = 0;
    int step = ceil(75 / lineLenght);
    for (int y = fromY; y < toY; y++) {
        if (y < 0 || y > height - 1) break;
        if (goDown) {
            pixColor.setBrightness(brightness + (b * step));
        } else {
            pixColor.setBrightness(brightness - (b * step));
        }
        source.setColor(x, y, pixColor);
        b++;
    }
    source.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    source.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}
