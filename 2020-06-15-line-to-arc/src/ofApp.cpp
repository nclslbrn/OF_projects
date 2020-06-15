#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 16);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    ofNoFill();
    initRadius = ofGetWidth() / 1.5;
    positions.resize(numArcs);
    distances.resize(numArcs);
    thetas.resize(numArcs);
    float radius = initRadius;
    ofVec2f lastV = ofVec2f(ofGetWidth() / 2 + radius * glm::cos(-circleRes), ofGetHeight() / 2 + radius * glm::sin(-circleRes));
    for (int a = 0; a < numArcs; a++) {
        float theta = circleRes * a;
        ofVec2f v = ofVec2f(ofGetWidth() / 2 + radius * glm::cos(theta), ofGetHeight() / 2 + radius * glm::sin(theta));
        float dist = lastV.distance(v);
        positions[a] = v;
        distances[a] = dist;
        thetas[a] = theta - (circleRes / 1.3);
        lastV = v;
        radius *= 0.9968;
    }
}
//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableAlphaBlending();
    ofBackground(0);

    for (int n = 0; n < samplesPerFrame; n++) {
        float alpha = (float)n / (float)samplesPerFrame;
        float t = ofMap(currFrame + n * shutterAngle / (float)samplesPerFrame, 0, numFrames, 0, 1);
        //float t = ((currFrame - n) % numFrames) / static_cast<float>(numFrames);
        ofSetColor(255.0f * alpha);
        animation(t);
        ofDrawBitmapString(ofToString(alpha), 8, ofGetHeight() - 16);
        //std::cout << alpha << endl;
    }
    ofDisableAlphaBlending();

    if (isDebugActive) {
        for (int a = 0; a < positions.size() - 1; a++) {
            ofDrawLine(
                positions[a].x,
                positions[a].y,
                positions[a].x + distances[a] * glm::cos(thetas[a]),
                positions[a].y + distances[a] * glm::sin(thetas[a]));
        }
    }
    if (currFrame < numFrames) {
        currFrame++;

        if (isRecording) {
            img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("line-to-arc" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
    }

    ofPushStyle();
    ofSetColor(255, 0, 0);
    if (willRecord) ofDrawBitmapString("Record will start", 16, 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);
    ofPopStyle();
}
//-------------------------------------------------------------
void ofApp::animation(float t) {
    float te = ease(t);
    float et = ease(1 - t);
    for (int a = 1; a < positions.size(); a++) {
        ofBeginShape();
        for (int b = 0; b <= distances[a]; b++) {
            float theta1 = b / static_cast<float>(distances[a]) * glm::pi<float>();
            if (t < 0.5) {
                ofVertex(
                    positions[a].x + b * glm::cos(thetas[a] + (theta1 * te)),
                    positions[a].y + b * glm::sin(thetas[a] + (theta1 * te)));
            } else {
                ofVertex(
                    positions[a].x + b * glm::cos(thetas[a] - (theta1 * et)),
                    positions[a].y + b * glm::sin(thetas[a] - (theta1 * et)));
            }
        }
        ofEndShape(false);
    }
}
//--------------------------------------------------------------
void ofApp::onGifSaved(string& fileName) {
    isSaved = true;
    cout << "Gif saved as " << fileName << endl;
    isSaved = false;
    isOptimizing = true;
    string path = ofFilePath::getCurrentExeDir() + "data/";
    string command = "gifsicle -O3 < " + path + fileName + " > " + path + "/opt-" + fileName;
    ofSystem(command);
    cout << "GifSicle optimized version saved as opt-" << fileName << endl;
    isOptimizing = false;
    isExported = true;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {  // s
        isExported = false;
        willRecord = true;
    } else if (key == 100) {  // d
        isDebugActive = !isDebugActive;
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}
void ofApp::exit() {
    gifEncoder.exit();
}
