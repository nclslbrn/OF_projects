#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
ofVec2f ofApp::circle(float n) {
    return ofVec2f(glm::cos(n), glm::sin(n));
}
//--------------------------------------------------------------
// https://www.wolframalpha.com/widgets/view.jsp?id=4e37f43fcbe8be03c20f977f32e20d15
//--------------------------------------------------------------
ofVec2f ofApp::astroid(float n) {
    return ofVec2f(
        pow(glm::sin(n), 3),
        pow(glm::cos(n), 3));
}
//--------------------------------------------------------------
ofVec2f ofApp::quadrifolium(float n) {
    return ofVec2f(
        glm::sin(n * 2) * glm::cos(n),
        glm::cos(n) * glm::sin(n * 2));
}
//--------------------------------------------------------------
ofVec2f ofApp::rect_hyperbola(float n) {
    float sec = 1 / glm::sin(n);

    float xt = 1 / glm::sin(n);
    float yt = glm::tan(n);

    return ofVec2f(xt, yt);
}
//--------------------------------------------------------------
ofVec2f ofApp::trifolium(float n) {
    return ofVec2f(
        -1.0f * glm::cos(n) * glm::cos(3.0 * n),
        glm::sin(n) - glm::cos(3.0f * n));
}
//--------------------------------------------------------------
void ofApp::setup() {
    for (float x = -3; x <= 3; x += density) {
        for (float y = -3; y <= 3; y += density) {
            pos.push_back(ofVec2f(
                x + ofRandom(1) * 0.03,
                y + ofRandom(1) * 0.03));
            colors.push_back(floor(palette.size() * ofRandom(1)));
        }
    }
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    cache.begin();
    ofClear(0);
    cache.end();
}

//--------------------------------------------------------------
void ofApp::update() {
    cache.begin();
    int index = 0;
    float t = ofGetFrameNum() / 100;
    for (ofVec2f& p : pos) {
        float xx = ofMap(p.x, -6.5, 6.5, (width / 2.0) - width, (width / 2.0) + width);
        float yy = ofMap(p.y, -6.5, 6.5, (height / 2.0) - height, (height / 2.0) + height);
        float rx = 2 * glm::cos(glm::pi<float>() * t);
        float ry = 2 * glm::sin(glm::pi<float>() * t);

        // third noise arg: t * glm::two_pi<float>()
        float n1 = 2 * ofMap(ofNoise(1 + p.x / 20.0f, 1 + p.y / 20.0f, rx, ry), 0, 1, -1, 1);
        //float n1a = 300 * ofMap(ofNoise(p.x / 10, p.y / 10, rx, ry), 0, 1, -1, 1);
        //float n1b = 300 * ofMap(ofNoise(p.y / 10, p.x / 10, rx, ry), 0, 1, -1, 1);

        /*    ofVec2f v1 = rect_hyperbola(n1a);
        ofVec2f v2 = astroid(n1b);
        ofVec3f v3 = quadrifolium(n1a); */
        ofVec3f v4 = trifolium(n1);

        /*
        ofVec2f diff1 = v2 - v1;
        ofVec2f diff2 = v4 - v3;
        diff1 *= 0.3;
        diff2 *= 0.3; */

        ofVec2f v = v4;
        p.x += vectorScale * v.x;
        p.y += vectorScale * v.y;

        ofSetColor(
            palette[colors[index]].r,
            palette[colors[index]].g,
            palette[colors[index]].b,
            50);
        ofDrawCircle(xx, yy, 0.5);

        index++;
    }
    cache.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    cache.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
