#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::initConstants() {
    A = ofRandom(4) - 2;
    B = ofRandom(4) - 2;
    C = ofRandom(4) - 2;
    D = ofRandom(4) - 2;

    std::cout << "A: " << ofToString(A) << " B:" << ofToString(B) << " C:" << ofToString(C) << " D:" << ofToString(D) << endl;
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);

    // Generate new constants for attracors
    initConstants();
    // Set start position
    position.set(0, 0);
    // Set scale for polar coordinate
    minX = -4.0;
    minY = minX * ofGetHeight() / ofGetWidth();
    maxX = 4.0;
    maxY = maxX * ofGetHeight() / ofGetWidth();

    img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
    img.setColor(ofColor::black);

    // GUI
    reinitConstantsButton.addListener(this, &ofApp::reinitConstantsButtonClick);
    saveConstantsButton.addListener(this, &ofApp::saveConstantsButtonClick);
    gui.setup("Constants controls");
    gui.add()
}

//--------------------------------------------------------------
void ofApp::update() {
    if (i < iterations) {
        float xn = glm::sin(A * position.y) + C * glm::cos(A * position.x);
        float yn = glm::sin(B * position.x) + D * glm::cos(B * position.y);

        float xi = (position.x - minX) * ofGetWidth() / (maxX - minX);
        float yi = (position.y - minY) * ofGetHeight() / (maxY - minY);

        // skip the first ten point
        if (i > 10) {
            img.setColor(xi, yi, ofColor::white);
        }
        // save for the next ieration
        position.x = xn;
        position.y = yn;
        i++;
    } else {
        img.save("A" + ofToString(A) + "B" + ofToString(B) + "C" + ofToString(C) + "D" + ofToString(D) + ".png");
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    img.draw(0, 0);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::reinitConstantsButtonClick() {
    initConstants();
}
//--------------------------------------------------------------
void ofApp::saveConstantsButtonClick() {
    // TODO
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
