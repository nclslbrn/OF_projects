#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Generate new constants for attracors
    A = ofRandom(4) - 2;
    B = ofRandom(4) - 2;
    C = ofRandom(4) - 2;
    D = ofRandom(4) - 2;
    // Set start position
    position.set(0, 0);
    // Set scale for polar coordinate
}

//--------------------------------------------------------------
void ofApp::update() {
    if (i < iterations) {
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
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
