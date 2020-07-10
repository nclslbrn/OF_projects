#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    ofNoFill();

    center = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
}

//--------------------------------------------------------------
void ofApp::update() {
    cache.begin();
    ofClear(0, 50);
    ofBackground(0, 50);
    for (int n = 0; n < samplesPerFrame; n++) {
        float alpha = 1.0 - (float)n / (float)samplesPerFrame;
        float t = ofMap((currFrame + n * shutterAngle / (float)samplesPerFrame), 0, numFrames, 0, 1);
        ofSetColor(255, 255, 255, (int)(50.0 * alpha));
        animation(t);
    }
    cache.end();
    if (currFrame < numFrames) {
        currFrame++;

        if (isRecording) {
            ofPixels pixels;
            cache.readToPixels(pixels);
            img.setFromPixels(pixels);
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("double-rotation" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cache.draw(0, 0);

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
void ofApp::animation(float t) {
    //float tt = t < 0.5 ? t : 1 - t;
    int i = 0;
    vector<ofVec2f> ps;
    ps.resize(arcs * n);
    ps.assign(arcs * n, ofVec2f(0, 0));
    ofVec2f p, pn, pn1;

    for (
        float theta1 = 0.0f;
        theta1 <= glm::two_pi<float>();
        theta1 += glm::two_pi<float>() / static_cast<float>(arcs)) {
        float theta2 = glm::pi<float>() * t;
        ofVec2f p1 = ofVec2f(
            center.x + radius * glm::cos(theta1 + theta2),
            center.y + radius * glm::sin(theta1 + theta2));
        ofDrawCircle(p1.x, p1.y, 8);
        if (p.x != 0) {
            ofDrawLine(p.x, p.y, p1.x, p1.y);
        }
        pn1 = p = p1;

        for (int j = n; j > -1; j--) {
            float j_n = j / static_cast<float>(n);
            float lambda = glm::two_pi<float>() * i / static_cast<float>(arcs) + glm::pi<float>() * j_n;
            float theta3 = glm::pi<float>() + theta1 + lambda;

            ofVec2f p2 = ofVec2f(
                pn1.x + (arcLength * glm::sin(j_n)) * glm::cos(theta3),
                pn1.y + (arcLength * glm::sin(j_n)) * glm::sin(theta3));

            if (ps[j].x != 0) {  //&& ps[j - 1].x != 0) {
                ofDrawLine(pn1.x, pn1.y, p2.x, p2.y);
                ofDrawLine(p2.x, p2.y, ps[j].x, ps[j].y);
            }
            pn1 = p2;
            ps[j] = p2;
        }
        i++;
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
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}
void ofApp::exit() {
    gifEncoder.exit();
}
