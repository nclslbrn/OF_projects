#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    //ofSetFrameRate(25);
    cellWidth = ofGetWidth() / static_cast<float>(cols);
    cellHeight = ofGetHeight() / static_cast<float>(rows);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 128);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    float t = (currFrame % animFrame) / static_cast<float>(animFrame);
    int total = rows * cols;

    for (int x = 0; x <= cols; x++) {
        for (int y = 0; y <= rows; y++) {
            int nx = x < cols / 2 ? x : cols - x;
            int ny = y < rows / 2 ? y : rows - y;

            float noiseValue = ofNoise(
                nx * cellWidth / noiseScale,
                ny * cellHeight / noiseScale,
                glm::cos(t * glm::two_pi<float>()),
                glm::sin(t * glm::two_pi<float>()));

            ofSetColor(ofFloatColor(noiseValue, 1));
            ofFill();
            ofDrawRectangle(
                x * cellWidth,
                y * cellHeight,
                cellWidth,
                cellHeight);
            /*  
            ofSetColor(ofColor(255 - hue));
            ofDrawBitmapString(
                "[" + ofToString(nx) + ":" + ofToString(ny) + "]",
                (x * cellWidth) + (cellWidth / 4),
                (y * cellHeight) + (cellHeight / 2)); 
            */
        }
    }
    if (isRecording && currFrame < animFrame) {
        img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        gifEncoder.addFrame(img, 0.033f);
    }
    if (currFrame == animFrame) {
        if (isRecording) {
            gifEncoder.save("Dancing-line" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        currFrame = 0;
    }
    ofDrawBitmapString("Scale: " + ofToString(noiseScale), 16, ofGetHeight() - 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);

    currFrame++;
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
    }
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    noiseScale = ofMap(y, 0, ofGetHeight(), 32, 1260);
}
