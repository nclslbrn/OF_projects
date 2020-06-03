#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    margin = ofVec2f((ofGetWidth() * marginFactor) / 2, (ofGetHeight() * marginFactor) / 2);
    initCellWidth = (ofGetWidth() - margin.x) / (columns * 2);
    initCellHeight = (ofGetHeight() - margin.y) / (rows * 2);

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            points.push_back(ofVec2f(
                x * initCellWidth,
                y * initCellHeight));
        }
    }
    initPoints = points;

    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 2);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
}

//--------------------------------------------------------------
void ofApp::update() {
    float t = (currFrame % animFrame) / static_cast<float>(animFrame);
    for (int p = 0; p < points.size(); p++) {
        float noise = ofNoise(
            points[p].x / noiseScale,
            points[p].y / noiseScale,
            glm::cos(t * glm::two_pi<float>()),
            glm::sin(t * glm::two_pi<float>()));

        points[p].x = initPoints[p].x + glm::cos(noise) * radius;
        points[p].y = initPoints[p].y + glm::sin(noise) * radius;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    int fromRight = ofGetWidth() - margin.x * 2;
    int fromBottom = ofGetHeight() - margin.y * 2;

    ofBackground(0);
    //float t = ofMap(ofGetFrameNum() % animFrame, 0, animFrame, 0, 1);
    ofPushMatrix();
    ofTranslate(margin.x, margin.y);
    for (int p = 0; p < points.size(); p++) {
        // horizontal line
        if (p % columns != columns - 1 && p + 1 != (int)points.size()) {
            // top left
            ofDrawLine(points[p].x, points[p].y, points[p + 1].x, points[p + 1].y);
            // top right
            ofDrawLine(fromRight - points[p].x, points[p].y, fromRight - points[p + 1].x, points[p + 1].y);
            // bottom left
            ofDrawLine(points[p].x, fromBottom - points[p].y, points[p + 1].x, fromBottom - points[p + 1].y);
            // bottom right
            ofDrawLine(fromRight - points[p].x, fromBottom - points[p].y, fromRight - points[p + 1].x, fromBottom - points[p + 1].y);
        }
        // vertical lines
        if (p + columns <= points.size() - 1) {
            // top left
            ofDrawLine(points[p].x, points[p].y, points[p + columns].x, points[p + columns].y);
            // top right
            ofDrawLine(fromRight - points[p].x, points[p].y, fromRight - points[p + columns].x, points[p + columns].y);
            // bottom left
            ofDrawLine(points[p].x, fromBottom - points[p].y, points[p + columns].x, fromBottom - points[p + columns].y);
            // bottom right
            ofDrawLine(fromRight - points[p].x, fromBottom - points[p].y, fromRight - points[p + columns].x, fromBottom - points[p + columns].y);
        }
    }
    ofPopMatrix();

    if (isRecording && currFrame < animFrame) {
        img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        gifEncoder.addFrame(img, 0.033f);
    }
    if (currFrame >= animFrame) {
        if (isRecording) {
            gifEncoder.save("Dancing-grid" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        currFrame = 0;
    }

    ofDrawBitmapString("Radius : " + ofToString(radius), 16, ofGetHeight() - 32);
    ofDrawBitmapString("Scale: " + ofToString(noiseScale), 16, ofGetHeight() - 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);

    currFrame++;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    radius = ofMap(x, 0, ofGetWidth(), 4, 126);
    noiseScale = ofMap(y, 0, ofGetHeight(), 32, 600);
}
//--------------------------------------------------------------
void ofApp::onGifSaved(string &fileName) {
    isSaved = true;
    cout << "Gif saved as " << fileName << endl;
    isSaved = false;
    isOptimizing = true;
    string path = ofFilePath::getCurrentExeDir() + "data/";
    string command = "gifsicle -O3 < " + path + fileName + " > " + path + "opt-" + fileName;
    cout << "GifSicle optimized version saved as opt-" << fileName << endl;
    ofSystem(command);
    isOptimizing = false;
    isExported = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {
        isExported = false;
        isRecording = true;
    } else if (key == 27) {
        exit();
    } else {
        ofLogNotice() << "Unassigned key (" << key << ")";
    }
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}