#include "ofApp.h"
//--------------------------------------------------------------
// Search and return bright pixel above y
//--------------------------------------------------------------
int ofApp::nextBrightY(int x, int y) {
    while (y >= 0) {
        ofColor c = picture.getColor(x, y);
        if (c.getBrightness() > brightThreshold) {
            return y;
        }
        y--;
    }
    return -1;
}
//--------------------------------------------------------------
// Search and return dark pixel below y
//--------------------------------------------------------------
int ofApp::nextDarkY(int x, int y) {
    while (y <= height) {
        ofColor c = picture.getColor(x, y);
        if (c.getBrightness() < darkThreshold) {
            return y;
        }
        y++;
    }
    return -1;
}
//--------------------------------------------------------------
void ofApp::setup() {
    picture.load("800x800/" + sourceImage);
    width = picture.getWidth();
    height = picture.getHeight();
    currY = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (currY <= height) {
        int y = currY;
        bool isLooking = true;
        while (isLooking && y <= height) {
            for (int x = 0; x < width; x++) {
                ofColor c = picture.getColor(x, y);
                float brightness = c.getBrightness();
                if (brightness < darkThreshold) {
                    // dark to  bright
                    addLine(x, y, c, brightness, true);
                    isLooking = false;
                } else if (brightness > brightThreshold) {
                    // bright to dark
                    addLine(x, y, c, brightness, false);
                    isLooking = false;
                }
            }
            y++;
        }
        currY = y;
    } else if (!isSaved) {
        exit();
    }
}

//--------------------------------------------------------------
void ofApp::addLine(int x, int y, ofColor pixColor, int brightness, bool searchBrightPix) {
    int toY = searchBrightPix ? nextBrightY(x, y) : nextDarkY(x, y);
    if (toY == -1) {
        toY = searchBrightPix ? y + ofRandom(lineLenght) : y - ofRandom(lineLenght);
    }
    int range = abs(toY - y);
    int step = floor(brightnessVariation / static_cast<float>(range));
    for (int d = 0; d < range; d++) {
        // bright go down, dark go up
        int y_ = toY - y < 0 ? y + d : y - d;
        if (y_ >= 0 && y_ <= height) {
            if (searchBrightPix) {
                pixColor.setBrightness(brightness + (step * d));
            } else {
                pixColor.setBrightness(brightness - (step * d));
            }
            picture.setColor(x, y_, pixColor);
        }
    }
    picture.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    picture.draw(0, 0);

    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofFill();
    ofDrawRectangle(ofGetWidth() / 2 - 4, currY - 1, 8, 2);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {  // s
        picture.save("output/" + sourceImage);
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}
void ofApp::exit() {
    picture.save("output/" + ofToString(width) + "-" + sourceImage);
    isSaved = true;
    ofExit();
}
