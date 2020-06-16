#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    // slowly
    ofSetFrameRate(12);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    font.load("fonts/OperatorMono-Medium.otf", 16);
    pos.resize(size);
    strings.resize(size);
    for (int a = 0; a < size; a++) {
        char id[2];
        id[0] = ofMap(ofRandom(1), 0, 1, 'A', 'Z');
        id[1] = ofMap(ofRandom(1), 0, 1, '1', '9');
        pos[a] = ofVec2f(ofRandom(1), ofRandom(1));
        strings[a] = id;
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    cache.begin();
    ofClear(0);
    ofBackground(0);
    for (int n = 0; n < samplesPerFrame; n++) {
        float alpha = 1.0 - (float)n / (float)samplesPerFrame;
        float t = ofMap((currFrame + n * shutterAngle / (float)samplesPerFrame), 0, numFrames, 0, 1);
        //float t = ((currFrame + n) % numFrames) / static_cast<float>(numFrames);
        ofSetColor(255, 255, 255, (int)(150.0 * alpha));
        animation(t);
    }
    cache.end();

    if (currFrame < numFrames) {
        currFrame++;

        if (isRecording) {
            ofPixels pixels;
            cache.readToPixels(pixels);
            img.setFromPixels(pixels);
            //img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("vector-scaling" + ofGetTimestampString() + ".gif");
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
    float te = ease(t + t);
    float et = ease(2.0 - (t + t));
    split = ofVec2f(t < 0.5 ? te : et, t < 0.5 ? te : et);

    for (int a = 0; a < size; a++) {
        ofVec2f remap;
        if (pos[a].x <= split.x) {
            remap.x = ofMap(pos[a].x, 0, 0.5, 0, ofGetWidth() * split.x);
        } else {
            remap.x = ofMap(pos[a].x, 0.5, 1, ofGetWidth() * split.x, ofGetWidth());
        }
        if (pos[a].y <= split.y) {
            remap.y = ofMap(pos[a].y, 0, 0.5, 0, ofGetHeight() * split.y);
        } else {
            remap.y = ofMap(pos[a].y, 0.5, 1, ofGetHeight() * split.y, ofGetHeight());
        }
        //ofScale(t + t);
        font.drawString(strings[a], remap.x, remap.y);
        //ofDrawBitmapString();
    }
    ofDrawLine(split.x * ofGetWidth(), 0, split.x * ofGetWidth(), ofGetHeight());
    ofDrawLine(0, split.y * ofGetHeight(), ofGetWidth(), split.y * ofGetHeight());

    ofDrawRectangle(split.x * ofGetWidth() - 1, split.y * ofGetHeight() - 5, 2, 10);
    ofDrawRectangle(split.x * ofGetWidth() - 5, split.y * ofGetHeight() - 1, 10, 2);
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

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    //split = ofVec2f(x / (float)ofGetWidth(), y / (float)ofGetHeight());
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
void ofApp::exit() {
    gifEncoder.exit();
}
