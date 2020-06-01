#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    xMargin = (ofGetWidth() * margin) / 2;
    yMargin = (ofGetHeight() * margin) / 2;
    initCellWidth = (ofGetWidth() - xMargin) / columns;
    initCellHeight = (ofGetHeight() - yMargin) / rows;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            points.push_back(ofVec2f(
                xMargin + x * initCellWidth,
                yMargin + y * initCellHeight));
        }
    }
    initPoints = points;
}

//--------------------------------------------------------------
void ofApp::update() {
    float t = ofMap(ofGetFrameNum() % numFrame, 0, numFrame, 0, 1);

    for (int p = 0; p < points.size(); p++) {
        float noise = ofNoise(
            points[p].x / noiseScale,
            points[p].y / noiseScale,
            glm::cos(t * glm::two_pi<float>()),
            glm::sin(t * glm::two_pi<float>()));

        points[p].x = initPoints[p].x + glm::cos(noise) * radius;
        points[p].y = initPoints[p].y + glm::sin(noise) * radius;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    for (int p = 0; p < points.size(); p++) {
        if (p % columns != columns - 1 && p + 1 != (int)points.size()) {
            ofDrawLine(
                points[p].x,
                points[p].y,
                points[p + 1].x,
                points[p + 1].y);
        }
        if (p + columns <= points.size() - 1) {
            ofDrawLine(
                points[p].x,
                points[p].y,
                points[p + columns].x,
                points[p + columns].y);
        }
        ofDrawCircle(points[p].x, points[p].y, 3);
        ofDrawBitmapString(p + 1, points[p].x, points[p].y);
    }
    ofDrawBitmapString("Radius : " + ofToString(radius), 16, ofGetHeight() - 32);
    ofDrawBitmapString("Scale: " + ofToString(noiseScale), 16, ofGetHeight() - 16);
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    radius = ofMap(x, 0, ofGetWidth(), 4, 126);
    noiseScale = ofMap(y, 0, ofGetHeight(), 32, 600);
}
