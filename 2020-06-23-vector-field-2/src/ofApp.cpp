#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }

//--------------------------------------------------------------
// https://www.wolframalpha.com/widgets/view.jsp?id=4e37f43fcbe8be03c20f977f32e20d15
//--------------------------------------------------------------

ofVec2f ofApp::circle(float n) {
    return ofVec2f(glm::cos(n), glm::sin(n));
}
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
    return ofVec2f(
        glm::sec(n),
        glm::tan(n));
}
//--------------------------------------------------------------
ofVec2f ofApp::trifolium(float n) {
    return ofVec2f(
        -1.0f * glm::cos(n) * glm::cos(3.0 * n),
        glm::sin(n) - glm::cos(3.0f * n));
}
//--------------------------------------------------------------
ofVec2f ofApp::cardioid(float n) {
    return ofVec2f(
        1.0f - glm::cos(n) * glm::cos(n),
        glm::sin(n) * (1.0f - glm::cos(n)));
}
//--------------------------------------------------------------
ofVec2f ofApp::deltoid(float n) {
    return ofVec2f(
        (2 * cos(n) / 3.0f) + (1 / 3.0f * cos(2 * n)),
        (2 * sin(n) / 3.0f) - (1 / 3.0f * sin(2 * n)));
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
    float t = ofGetFrameNum() % numFrame / numFrame;
    for (ofVec2f& p : pos) {
        float xx = ofMap(p.x, -6.5, 6.5, -width / 3, width * 1.33);
        float yy = ofMap(p.y, -6.5, 6.5, -height / 3, height * 1.33);

        //float n1 = 20 * ofMap(ofNoise(p.x / 5.0f, p.y / 5.0f), 0, 1, -1, 1);
        float n1a = 5 * ofMap(ofNoise(p.x / 5.0f, p.y / 5.0f), 0, 1, -1, 1);
        float n1b = 20 * ofMap(ofNoise(p.y / 5.0f, p.x / 5.0f), 0, 1, -1, 1);

        ofVec2f v1 = rect_hyperbola(n1a);
        //ofVec2f v2 = astroid(n1a);
        ofVec3f v3 = deltoid(n1b);
        //  ofVec3f v4 = deltoid(p.y);
        //float n2a = 30 * ofMap(ofNoise(v2.x, v2.y), 0, 1, -1, 1);
        float n2b = 5.0f * ofMap(ofNoise(v3.x, v3.y, t * 5), 0, 1, -1, 1);
        float n3b = 5.0f * ofMap(ofNoise(v1.x, v1.y, t * 5), 0, 1, -1, 1);

        ofVec2f v4 = astroid(n2b);
        ofVec2f v5 = cardioid(n3b);
        ofVec2f v = ofVec2f(v4.x, v5.y);
        //ofVec2f v = v5 + v4;
        p.x += vectorScale * v.x;
        p.y += vectorScale * v.y;

        ofSetColor(
            palette[colors[index]].r,
            palette[colors[index]].g,
            palette[colors[index]].b,
            10);
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
    std::cout << "key pressed = " << key << endl;
    if (key == 115) {
        ofPixels pixels;
        ofImage output;
        cache.readToPixels(pixels);
        output.setFromPixels(pixels);
        output.save("output-" + ofToString(ofGetFrameNum()) + ".jpg");
    }
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
