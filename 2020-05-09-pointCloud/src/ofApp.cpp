#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    ofSetSmoothLighting(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    streetModel.load("plys/GT_Madame1_2-from-meshlab.ply");

    point.setDiffuseColor(ofColor(255.0, 120.0, 120.0));
    point.setPointLight();

    shader.load("shadersGL3/shader");
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0, 0, 0);
    cam.begin();
    ofSetColor(200, 200, 200, 255);
    point.setPosition(0, 0, (float)ofGetHeight() * 0.75);
    point.enable();
    shader.begin();
    streetModel.drawVertices();
    shader.end();
    point.disable();
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
