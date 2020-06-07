#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // remove line below before export GIF
    ofSetFrameRate(25);
    cellWidth = ofGetWidth() / static_cast<float>(cols);
    cellHeight = ofGetHeight() / static_cast<float>(rows);
    cellDiag = sqrt(pow(cellWidth, 2) + pow(cellHeight, 2));
    midWidthGrid = floor(cols / 2);
    midHeightGrid = floor(rows / 2);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 128);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

    linePoints.clear();
    linePoints.resize(cols * rows);
    for (int n = 0; n < cols * rows; n++) {
        linePoints[n].resize(liveFrame);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            int i = (y * cols) + x;

            for (int f = 0; f < liveFrame; f++) {
                float t = ((currFrame + f) % animFrame) / static_cast<float>(liveFrame);
                /* Cast x and y to make noise symmetrical 
                int nx = x <= midWidthGrid - 1 ? x : cols - x - 1;
                int ny = y <= midHeightGrid - 1 ? y : rows - y - 1;
                int n = (ny * midWidthGrid) + nx; 
                */

                float noise = ofNoise(
                    x * cellWidth / noiseScale,
                    y * cellHeight / noiseScale,
                    glm::cos(t * glm::two_pi<float>()),
                    glm::sin(t * glm::two_pi<float>()));

                linePoints[i][f] = ofVec2f(
                    x * cellWidth + (glm::cos(noise) * noiseRadius),
                    y * cellHeight + (glm::sin(noise) * noiseRadius));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    //ofNoFill();
    ofPushMatrix();
    ofScale(0.8);
    ofTranslate(-cellWidth / 2, -cellHeight / 2, 0);
    ofPushStyle();
    for (int p = 0; p < cols * rows; p++) {
        ofSetColor(ofColor::fromHsb(255 * (p / static_cast<float>(cols * cols)), 255, 255, 255));
        ofBeginShape();
        for (int f = 0; f < liveFrame; f++) {
            int d = (f / static_cast<float>(liveFrame)) * cellDiag;
            ofVertex(linePoints[p][f].x + d, linePoints[p][f].y + d);
        }
        ofEndShape();
    }
    ofPopStyle();
    ofPopMatrix();

    if (currFrame < animFrame - 1) {
        currFrame++;

        if (isRecording) {
            img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
        if (isRecording) {
            gifEncoder.save("Dancing-line" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
    }
    ofDrawBitmapString("Frame: " + ofToString(currFrame) + "/" + ofToString(animFrame), 16, ofGetHeight() - 48);
    ofDrawBitmapString("Radius: " + ofToString(noiseRadius), 16, ofGetHeight() - 32);
    ofDrawBitmapString("Scale: " + ofToString(noiseScale), 16, ofGetHeight() - 16);

    ofPushStyle();
    ofSetColor(255, 0, 0);
    if (willRecord) ofDrawBitmapString("Record will start", 16, 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);
    ofPopStyle();
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
        willRecord = false;
        isRecording = true;
    }
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    noiseRadius = x;
    noiseScale = y + 1;
}
