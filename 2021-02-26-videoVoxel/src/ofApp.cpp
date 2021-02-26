#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(20);
    ofDisableArbTex();
    mesh.setMode(OF_PRIMITIVE_POINTS);

    videoSample.loadMovie("video/Pexels Videos 2053855.mp4");
    videoSample.setSpeed(0.25);

    //videoSample.setLoopState(OF_LOOP_NORMAL);
    videoSample.play();
    nFrame = 150;
    res = 8;
    cam.setPosition(0, 0, 120);
    cam.setVFlip(true);
    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(3);
}

//--------------------------------------------------------------
void ofApp::update() {
    videoSample.update();

    if (videoSample.isFrameNew()) {
        frames.push_front(videoSample.getPixelsRef());
        if (frames.size() > nFrame) {
            frames.pop_back();
        }
    }
    if (!frames.empty()) {
        if (!imagePixels.isAllocated()) {
            imagePixels = frames[0];
        }
        int w = frames[0].getWidth();
        int h = frames[0].getHeight();
        voxels.clear();

        for (int y = 0; y < h; y += res) {
            for (int x = 0; x < w; x += res) {
                ofColor voxCol = frames[0].getColor(x, y);
                float brightness = voxCol.getBrightness();
                ofVec3f voxPos = ofVec3f(x, y, brightness / 2);
                float size = ((255 - brightness) / 255) * (res * 1.5);
                voxels.push_front(Voxel(voxPos, voxCol, size));
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    //ofLog(OF_LOG_NOTICE, "Voxel num" + ofToString(voxels.size()));
    cam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2, -ofGetHeight() / 1.5);
    for (int i = 0; i < voxels.size(); i++) {
        voxels[i].draw();
    }
    ofPopMatrix();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
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