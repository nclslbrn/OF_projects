#include "ofApp.h"
//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 156);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    ofNoFill();
    initRadius = ofGetWidth() / 1.5;
    positions.resize(numArcs);
    distances.resize(numArcs);
    thetas.resize(numArcs);
    init();
}
void ofApp::init() {
    isInitialized = false;
    radius = initRadius;
    ofVec2f lastV = ofVec2f(ofGetWidth() / 2 + radius * glm::cos(-circleRes), ofGetHeight() / 2 + radius * glm::sin(-circleRes));
    for (int a = 0; a < numArcs; a++) {
        float theta = circleRes * a;
        ofVec2f v = ofVec2f(ofGetWidth() / 2 + radius * glm::cos(theta), ofGetHeight() / 2 + radius * glm::sin(theta));
        float dist = lastV.distance(v);
        positions[a] = v;
        distances[a] = dist;
        thetas[a] = theta - (circleRes / 1.3f);
        lastV = v;
        radius *= 0.9968;
    }
    isInitialized = true;
}
//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    float t = (currFrame % numFrames) / static_cast<float>(numFrames);
    if (isInitialized) {
        float te = ease(t);
        float et = ease(1 - t);
        ofVec2f lastV = positions[0];
        for (int a = 1; a < positions.size(); a++) {
            ofVec2f currentV = positions[a];

            ofBeginShape();
            for (int b = 0; b <= distances[a]; b++) {
                float theta1 = b / static_cast<float>(distances[a]) * glm::pi<float>();
                if (t < 0.5) {
                    ofVertex(
                        currentV.x + b * glm::cos(thetas[a] + (theta1 * te)),
                        currentV.y + b * glm::sin(thetas[a] + (theta1 * te)));
                } else {
                    ofVertex(
                        currentV.x + b * glm::cos(thetas[a] - (theta1 * et)),
                        currentV.y + b * glm::sin(thetas[a] - (theta1 * et)));
                }
            }
            ofEndShape(false);
            lastV = currentV;
        }
    }
    if (isDebugActive) {
        for (int a = 0; a < positions.size() - 1; a++) {
            ofVec2f currentV = positions[a];
            ofDrawLine(
                currentV.x,
                currentV.y,
                currentV.x + distances[a] * glm::cos(thetas[a]),
                currentV.y + distances[a] * glm::sin(thetas[a]));
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
    } else if (key == 32) {  // spacebar
        init();
    } else if (key == 100) {  // d
        isDebugActive = !isDebugActive;
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}
