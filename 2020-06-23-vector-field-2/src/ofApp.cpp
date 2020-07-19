#include "ofApp.h"
//--------------------------------------------------------------
// Dave Whyte (aka Bees and Bomb) ease function (original coded with Processing)
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
            colors.push_back(palette.getRandomColorId());
        }
    }
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
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
        float xx = ofMap(p.x, -6.5, 6.5, -width * canvasScale, width * (1 + canvasScale));
        float yy = ofMap(p.y, -6.5, 6.5, -height * canvasScale, height * (1 + canvasScale));

        //float n1 = 50 * ofMap(ofNoise(1 + p.x / 10.0f, 1 + p.y / 5.0f), 0, 1, -1, 1);

        ofVec2f v1 = deltoid(p.x);
        ofVec2f v2 = quadrifolium(p.y);
        float n2a = 5 * ofNoise(v1.x, v1.y);
        float n2b = 5 * ofNoise(v2.x, v2.y);
        float n3 = 10 * ofMap(ofNoise(n2a, n2b / 3.0f), 0, 1, -1, 1);
        //  ofVec2f v4 = trifolium(n2b);
        //  ofVec2f v5 = circle(n3b);
        ofVec2f v = circle(n3);
        //ofVec2f v = v5 + v4;
        p.x += vectorScale * v.x;
        p.y += vectorScale * v.y;

        ofSetColor(
            palette.getColor(colors[index]).r,
            palette.getColor(colors[index]).g,
            palette.getColor(colors[index]).b,
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
    if (key == 115) {
        ofPixels pixels;
        ofImage output;
        string filename = "output-" + ofToString(ofGetFrameNum()) + ".jpg";
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
