#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) {
    return 3 * p * p - 2 * p * p * p;
}
//--------------------------------------------------------------
float ofApp::softPlus(float q, float p) {
    float qq = q + p;
    if (qq <= 0) {
        return 0;
    }
    if (qq >= 2 * p) {
        return qq - p;
    }
    return 1 / (4 * p) * qq * qq;
}
//--------------------------------------------------------------
void ofApp::setup() {
    animFrame = 320;
    width = ofGetWidth();
    height = ofGetHeight();
    depth = 64;
    stepX = 8;
    stepY = 16;
    radius = stepX / 2;
    noiseScale = 0.0075;
    ofSetFrameRate(50);
    ofBackground(255, 255, 255);

    for (int x = 0; x < width; x += stepX) {
        for (int y = 0; y < height; y += stepY) {
            Line lineTemp;
            lineTemp.a = ofPoint(x, y, 0);
            lineTemp.b = ofPoint(x + stepX, y, 0);
            lines.push_back(lineTemp);
        }
    }
}
//--------------------------------------------------------------
void ofApp::update() {
    noisedLines.clear();
    int frameDrawn = (int)ofGetFrameNum() < animFrame ? (int)ofGetFrameNum() : (int)ofGetFrameNum() % animFrame;

    float t = ofMap(frameDrawn, 0, animFrame, 0, 1);

    for (auto line : lines) {
        float xDif = ofMap(
            line.a.x,
            line.a.x < width / 2 ? 0 : width / 2,
            line.a.x < width / 2 ? width / 2 : width,
            line.a.x < width / 2 ? 0 : 1,
            line.a.x < width / 2 ? 1 : 0);

        float yDif = ofMap(
            line.a.y,
            line.a.y < height / 2 ? 0 : height / 2,
            line.a.y < height / 2 ? height / 2 : height,
            line.a.y < height / 2 ? 0 : 1,
            line.a.y < height / 2 ? 1 : 0);

        float noise1 = ofNoise(
            line.a.x * noiseScale,
            line.a.y * noiseScale,
            radius * (glm::cos(t * (glm::pi<float>() * 2))),
            radius * (glm::sin(t * (glm::pi<float>() * 2))));

        float noise2 = ofNoise(
            line.b.x * noiseScale,
            line.b.y * noiseScale,
            radius * (glm::cos(t * (glm::pi<float>() * 2))),
            radius * (glm::sin(t * (glm::pi<float>() * 2))));

        float scale = depth * pow((yDif + xDif) / 2, 3);

        //std::cout << "xDif: " << ofToString(xDif) << endl;
        float y1 = line.a.y + glm::sin(noise1) * scale;
        float y2 = line.b.y + glm::sin(noise2) * scale;
        Line noisedlineTemp;
        noisedlineTemp.a = ofPoint(line.a.x, y1);
        noisedlineTemp.b = ofPoint(line.b.x, y2);

        noisedLines.push_back(noisedlineTemp);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofEnableAlphaBlending();
    ofSetColor(50, 50, 50, 120);
    ofSetLineWidth(2);

    ofPushMatrix();
    //ofTranslate(0, -height * 0.25, 0);
    //ofRotateX(320);
    // ofTranslate(-width / 2, -height / 2);
    for (int i = 0; i < noisedLines.size() - 1; i += 2) {
        Line line1 = noisedLines[i];
        Line line2 = noisedLines[i + 1];
        /*         
        mesh.addVertex(line1.a);
        mesh.addVertex(line2.a);
        mesh.addVertex(line2.b);
        mesh.addVertex(line1.b);

         */
        ofDrawLine(line1.a, line1.b);
        ofDrawLine(line2.a, line2.b);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    /*  translateX = ofGetMouseY();
        std::cout << "translateX: " << ofToString(translateX) << endl;
    */
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
