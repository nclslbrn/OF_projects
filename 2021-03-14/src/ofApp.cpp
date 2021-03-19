#include "ofApp.h"

#include "ofConstants.h"
//--------------------------------------------------------------
ofVec2f ofApp::getRandomPos(ofVec2f c, float scale) {
    return ofVec2f(
        ((ofRandomuf() * ofGetWidth()) - c.x) * scale,
        ((ofRandomuf() * ofGetHeight()) - c.y) * scale);
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofDisableArbTex();
    ofSetBackgroundColor(0);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    heatMap = MouseHeatMap(ofGetWidth(), ofGetHeight(), 50.0);
    frame = FrameMesh("nick_offerman-320x180px.jpg", 60, meshScale);
    shader.load("shader");
    shader.begin();
    shader.setUniformTexture("tex", frame.getTexture(), 0);
    shader.setUniform2f("u_frameRes", frame.getWidth(), frame.getHeight());
    shader.setUniform2f("u_touchRes", ofGetWidth(), ofGetHeight());
    shader.end();
    camera.setDistance(ofGetWidth() * 12);
    camera.setFarClip(ofGetWidth() * 12);

    repulsor.resize(2);
    repulsor[0] = getRandomPos(center, meshScale);
    repulsor[1] = getRandomPos(center, meshScale);
}

//--------------------------------------------------------------
void ofApp::update() {
    heatMap.capture(ofGetMouseX(), ofGetMouseY());
    if (ofGetFrameNum() % numFrame == 0) {
        repulsor[0] = repulsor[1];
        repulsor[1] = getRandomPos(center, meshScale);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    float t = (ofGetFrameNum() % numFrame) / static_cast<float>(numFrame);
    ofVec2f repulsT = repulsor[0].getInterpolated(repulsor[1], t);
    heatMap.bind();
    heatMap.draw(0, 0, ofGetWidth(), ofGetHeight());
    camera.begin();
    shader.begin();
    shader.bindDefaults();
    shader.setUniform1f("u_time", t);
    shader.setUniform2f("u_mouse", ofGetMouseX() - center.x, ofGetMouseY() - center.y);
    shader.setUniform3f("u_camera", camera.getGlobalPosition());
    shader.setUniform2f("u_repulsor", repulsT);
    shader.setUniformTexture("heatmap", heatMap.getMaptexture(), 1);
    ofPushMatrix();
    ofTranslate(center.x, center.y, 100);
    ofRotateX(180);
    frame.draw();
    ofPopMatrix();
    shader.end();
    camera.end();
    heatMap.unbind();

    ofDrawBitmapString(
        ofToString(ofGetFrameRate()) + " fps",
        20, 20);
    ofDrawBitmapString(
        ofToString(frame.getParticleNum()) + " particles",
        ofGetWidth() - 120, 20);
    ofDrawBitmapString(
        ofToString(t) + " / 1.0",
        20, ofGetHeight() - 20);
    ofDrawBitmapString(
        "[" + ofToString(repulsT.x) + ", " + ofToString(repulsT.y) + "]",
        ofGetWidth() - 200, ofGetHeight() - 20);
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
