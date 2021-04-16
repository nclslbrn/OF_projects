#include "ofApp.h"

#include "ofConstants.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofImage sampleImage;
    sampleImage.load("images/al-khwarizmi.jpg");
    ofPixels samplePixels = sampleImage.getPixels();
    sparkTexture.load("images/spark-2.png");
    sparkTexture.setImageType(OF_IMAGE_COLOR_ALPHA);
    shader.load("shaders/particle");

    imageMesh = FrameMesh(
        samplePixels,
        60,
        12,
        glm::vec2(sparkTexture.getWidth(), sparkTexture.getHeight()));
    imageMesh.compute(shader);

    camera.setVFlip(true);
    camera.setDistance(ofGetWidth());
    camera.setNearClip(0.1);
    camera.setFarClip(-12 * ofGetWidth() * 1.5);
    camera.setFov(45.0f);
    ofDisableArbTex();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    ofBackground(0);

    shader.begin();
    camera.begin();
    shader.bindDefaults();
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f);
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("u_time", ofGetFrameNum() / 1000.0f);
    shader.setUniformTexture("u_frameTex", imageMesh.getTexture(), 0);
    shader.setUniformTexture("u_sparkTex", sparkTexture.getTexture(), 1);
    imageMesh.drawPoints();
    ofPopMatrix();
    camera.end();
    shader.end();
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
