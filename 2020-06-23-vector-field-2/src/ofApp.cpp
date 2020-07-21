#include "ofApp.h"
//--------------------------------------------------------------
// Dave Whyte (aka Bees and Bomb) ease function (adapted from Processing version)
//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }

//--------------------------------------------------------------
// Plane curves function
// https://www.wolframalpha.com/widgets/view.jsp?id=4e37f43fcbe8be03c20f977f32e20d15
//--------------------------------------------------------------

ofVec2f ofApp::circle(float n) {
    return ofVec2f(glm::cos(n), glm::sin(n));
}
//--------------------------------------------------------------
ofVec2f ofApp::astroid(ofVec2f v) {
    return ofVec2f(
        pow(glm::sin(v.x), 3),
        pow(glm::cos(v.y), 3));
}
//--------------------------------------------------------------
ofVec2f ofApp::quadrifolium(ofVec2f v) {
    return ofVec2f(
        glm::sin(v.x * 2) * glm::cos(v.x),
        glm::cos(v.y) * glm::sin(v.y * 2));
}
//--------------------------------------------------------------
ofVec2f ofApp::rect_hyperbola(ofVec2f v) {
    return ofVec2f(
        glm::sec(v.x),
        glm::tan(v.y));
}
//--------------------------------------------------------------
ofVec2f ofApp::trifolium(ofVec2f v) {
    return ofVec2f(
        -1.0f * glm::cos(v.x) * glm::cos(3.0 * v.x),
        glm::sin(v.y) - glm::cos(3.0f * v.y));
}
//--------------------------------------------------------------
ofVec2f ofApp::cardioid(ofVec2f v) {
    return ofVec2f(
        1.0f - glm::cos(v.x) * glm::cos(v.x),
        glm::sin(v.y) * (1.0f - glm::cos(v.y)));
}
//--------------------------------------------------------------
ofVec2f ofApp::deltoid(ofVec2f v) {
    return ofVec2f(
        (2 * cos(v.x) / 3.0f) + (1 / 3.0f * cos(2 * v.x)),
        (2 * sin(v.y) / 3.0f) - (1 / 3.0f * sin(2 * v.y)));
}
//--------------------------------------------------------------
ofVec2f ofApp::ranunculoid(ofVec2f v) {
    return ofVec2f(
        6.0f * glm::cos(v.x) - glm::cos(6.0f * v.y),
        6.0f * glm::sin(v.y) - glm::sin(6.0f * v.y));
}
//--------------------------------------------------------------
ofVec2f ofApp::cycloid(ofVec2f v) {
    return ofVec2f(
        v.x - glm::sin(v.x),
        1.0f - glm::cos(v.y));
}
//--------------------------------------------------------------

void ofApp::setup() {
    for (float x = -3; x <= 3; x += density) {
        for (float y = -3; y <= 3; y += density) {
            pos.push_back(ofVec2f(
                x + ofRandom(1) * 0.03,
                y + ofRandom(1) * 0.03));
            colors.push_back(palette.getRandomColorId());
        }
    }
    cache.allocate(width, height, GL_RGB);
    cache.begin();
    ofClear(palette.getStroke());
    cache.end();

    std::cout << "Current palette is " << palette.getName() << "." << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
    cache.begin();
    int index = 0;
    float t = ofGetFrameNum() % numFrame / numFrame;
    for (ofVec2f& p : pos) {
        float xx = ofMap(p.x, -6.5, 6.5, 0, width);
        float yy = ofMap(p.y, -6.5, 6.5, 0, height);

        float n1 = 150.0f * ofMap(ofNoise(p.x / 5.0f, p.y / 5.0f, t), 0, 1, -1, 1);
        ofVec2f v1 = circle(n1);
        ofVec2f v2 = rect_hyperbola(v1);

        float n2 = ofMap(ofNoise(v2.x / 15.0f, v2.y / 15.0f), 0, 1, -1, 1);
        ofVec2f v3 = circle(n2) * 10.0f;
        ofVec2f v = astroid(v3);

        p.x += vectorScale * v.x;
        p.y += vectorScale * v.y;

        ofSetColor(
            palette.getColor(colors[index]).r,
            palette.getColor(colors[index]).g,
            palette.getColor(colors[index]).b,
            alpha);
        ofDrawCircle(xx, yy, 0.5);

        index++;
    }
    cache.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
    cache.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {
        ofPixels pixels;
        ofImage output;
        string filename = "output-" + ofToString(ofGetFrameNum()) + "--" + palette.getName() + ".jpg";
        cache.readToPixels(pixels);
        output.setFromPixels(pixels);
        output.save("output/" + filename);
        std::cout << "Image saved (" << filename << ")" << endl;
    } else {
        std::cout << "key pressed = " << key << endl;
    }
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
