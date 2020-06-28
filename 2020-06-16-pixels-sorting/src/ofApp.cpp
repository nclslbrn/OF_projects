#include "ofApp.h"
//--------------------------------------------------------------
// Search and return bright pixel above y
//--------------------------------------------------------------
int ofApp::nextBrightY(int x, int y, int brightness) {
    while (y >= 0) {
        ofColor c = source.getColor(x, y);
        if (c.getBrightness() >= brightness + currSetting.getQueryLigthThreshold()) {
            return y;
        }
        y--;
    }
    return -1;
}
//--------------------------------------------------------------
// Search and return dark pixel below y
//--------------------------------------------------------------
int ofApp::nextDarkY(int x, int y, int brightness) {
    while (y <= height) {
        ofColor c = source.getColor(x, y);
        if (c.getBrightness() <= brightness - currSetting.getQueryLigthThreshold()) {
            return y;
        }
        y++;
    }
    return -1;
}
//--------------------------------------------------------------
// Setup new setting to compute new image
//--------------------------------------------------------------
void ofApp::initSettings(int frameId) {
    if (frameId < settings.size()) {
        source.load(sourceSize + "/" + settings[frameId].getSourceName());
        source.setImageType(OF_IMAGE_COLOR);
        modified = source;
        width = source.getWidth();
        height = source.getHeight();
        currY = 0;
        isSaved = false;
        currSetting = settings[frameId];

    } else {
        std::cout << "No more frame to compute." << endl;
        ofExit();
    }
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofFile file("settings.json");
    if (file.exists()) {
        file >> jsonSettings;
        for (auto& setting : jsonSettings) {
            if (!setting.empty()) {
                Setting n = Setting(setting);
                settings.push_back(n);
            }
        }
    }
    initSettings(frameId);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (currY < height) {
        int y = currY;
        bool isLooking = true;
        while (isLooking && y <= height) {
            for (int x = 0; x < width; x += pixStep) {
                ofColor c = source.getColor(x, y);
                float brightness = c.getBrightness();
                if (brightness < currSetting.getDarkThreshold()) {
                    // dark to  bright
                    addLine(x, y, c, brightness, true);
                    isLooking = false;
                } else if (brightness > currSetting.getBrightThreshold()) {
                    // bright to dark
                    addLine(x, y, c, brightness, false);
                    isLooking = false;
                }
            }
            y++;
        }
        currY = y;
    } else if (!isSaved) {
        saveFrame();
        frameId++;
        initSettings(frameId);
    }
}

//--------------------------------------------------------------
void ofApp::addLine(int x, int y, ofColor pixColor, int brightness, bool searchBrightPix) {
    int toY = searchBrightPix ? nextBrightY(x, y, brightness) : nextDarkY(x, y, brightness);
    if (toY != -1) {
        int range = abs(toY - y);
        int step = floor(currSetting.getBrightnessVariation() / static_cast<float>(range));
        for (int d = 0; d < range; d++) {
            // bright go down, dark go up
            int y_ = toY - y < 0 ? y + d : y - d;
            if (y_ >= 0 && y_ < height) {
                if (searchBrightPix) {
                    pixColor.setBrightness(brightness + (step * d));
                } else {
                    pixColor.setBrightness(brightness - (step * d));
                }
                if (pixStep > 1) {
                    for (int x_ = x; x_ <= x + pixStep && x_ < width; x_++) {
                        modified.setColor(x_, y_, pixColor);
                    }
                } else if (x < width) {
                    modified.setColor(x, y_, pixColor);
                }
            }
        }
        modified.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    modified.draw(0, 0);

    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofFill();
    ofDrawRectangle(ofGetWidth() / 2 - 4, currY - 1, 8, 2);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    /*
    if (key == 115) {  // s
        modified.save("output/" + currSetting.getSourceName());
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
    */
}
void ofApp::saveFrame() {
    modified.save("output/" + sourceSize + "/" + currSetting.getSourceName());
    isSaved = true;
    std::cout << currSetting.getSourceName() << " saved" << endl;
}
