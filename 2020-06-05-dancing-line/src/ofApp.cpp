#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLineWidth(8);
    ofNoFill();

    cellWidth = ofGetWidth() / static_cast<float>(cols);
    cellHeight = ofGetHeight() / static_cast<float>(rows);
    cellDiag = sqrt(pow(cellWidth, 2) + pow(cellHeight, 2));
    //midWidthGrid = floor(cols / 2);
    //midHeightGrid = floor(rows / 2);
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

            for (int f = 0; f < animFrame; f++) {
                double t = (currFrame + f) % animFrame / static_cast<double>(animFrame);
                int d = (f / static_cast<float>(animFrame)) * cellDiag;

                /* Cast x and y to make 
                noise symmetrical 
                int nx = x <= midWidthGrid - 1 ? x : cols - x - 1;
                int ny = y <= midHeightGrid - 1 ? y : rows - y - 1;
                int n = (ny * midWidthGrid) + nx; 
                */

                float noise = ofNoise(
                    x * cellWidth / noiseScale,
                    y * cellHeight / noiseScale,
                    glm::cos(t * glm::pi<float>()),
                    glm::sin(t * glm::pi<float>()));

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
    ofScale(0.7);
    ofTranslate(-cellWidth / 2, -cellHeight / 2, 0);

    for (int p = 0; p < cols * rows; p++) {
        ofPushStyle();
        ofSetColor(lineColor[p]);
        ofBeginShape();
        for (int f = 0; f < animFrame; f++) {
            ofVertex(linePoints[p][f].x, linePoints[p][f].y);
        }
        ofEndShape(false);
        ofPopStyle();
    }
    if (currFrame < animFrame - 1) {
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
    noiseRadius = x / 2;
    noiseScale = (y + 1);
}
