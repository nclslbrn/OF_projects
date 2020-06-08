#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
float ofApp::ease(float p, float g) {
    if (p < 0.5) {
        return 0.5 * pow(2 * p, g);
    } else {
        return 1 - 0.5 * pow(2 * (1 - p), g);
    }
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLineWidth(8);
    ofNoFill();

    cellWidth = ofGetWidth() / static_cast<float>(cols);
    cellHeight = ofGetHeight() / static_cast<float>(rows);
    cellDiag = sqrt(pow(cellWidth, 2) + pow(cellHeight, 2));
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 156);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

    linePoints.clear();
    lineColor.clear();
    linePoints.resize(cols * rows);
    lineColor.resize(cols * rows);

    for (int n = 0; n < cols * rows; n++) {
        linePoints[n].resize(animFrame);

        int pointHue = static_cast<int>(
            255 * (n / static_cast<float>(cols * cols)));
        lineColor[n] = ofColor::fromHsb(pointHue, 120, 240);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            int i = (y * cols) + x;

            for (int f = 0; f <= animFrame; f++) {
                float t = ((currFrame + f) % animFrame) / static_cast<float>(animFrame);

                int d = (f / static_cast<float>(animFrame)) * cellDiag;
                float noise = ofNoise(
                    (x * cellWidth) - d / noiseScale,
                    (y * cellHeight) - d / noiseScale,
                    glm::cos(t * glm::two_pi<float>()),
                    glm::sin(t * glm::two_pi<float>()));

                linePoints[i][f] = ofVec2f(
                    (x * cellWidth + (glm::cos(noise) * noiseRadius)) + d,
                    (y * cellHeight + (glm::sin(noise) * noiseRadius)) + d);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    ofPushMatrix();
    ofScale(0.65);
    ofTranslate(-noiseRadius / 2, -noiseRadius / 2);
    ofVec2f lastPoint;
    for (int p = 0; p < cols * rows; p++) {
        ofPushStyle();
        ofSetColor(lineColor[p]);
        ofBeginShape();
        for (int f = 0; f <= animFrame; f++) {
            ofVertex(linePoints[p][f].x, linePoints[p][f].y);
        }
        ofEndShape(false);
        ofPopStyle();
        if (lastPoint.x != 0) {
            ofDrawLine(lastPoint.x, lastPoint.y, linePoints[p][animFrame].x, linePoints[p][animFrame].y);
        }
        lastPoint = linePoints[p][animFrame];
    }
    if (currFrame < animFrame) {
        currFrame++;

        if (isRecording) {
            img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }
    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("Dancing-line" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
    }
    ofPopMatrix();
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
void ofApp::onGifSaved(string& fileName) {
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
        willRecord = true;
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    noiseRadius = (x / 2) + 1;
    noiseScale = y + 1;
}
