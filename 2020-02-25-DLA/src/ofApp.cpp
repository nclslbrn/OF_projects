#include "ofApp.h"
//--------------------------------------------------------------
ofVec3f ofApp::randomPos() {
    return ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()),
                   ofRandom(ofGetHeight()));
}
//--------------------------------------------------------------
ofVec3f ofApp::onRadius() {
    ofVec3f center = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, ofGetHeight() / 2);
    float theta = ofRandom(1) * glm::pi<float>() * 2;
    float phi = glm::acos(1 - ofRandom(1) * 20);

    center.x += glm::sin(phi) * glm::cos(theta) * radius;
    center.y += glm::sin(phi) * glm::sin(theta) * radius;
    center.z += glm::cos(phi) * radius;

    return center;
}
//--------------------------------------------------------------
void ofApp::setup() {
    radius = min(ofGetWidth(), ofGetHeight()) / 3;
    steps = 100;
    walkerNum = 500;
    walkerSize = 8;
    decrease = 0.95;
    ofVec3f center = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, ofGetHeight() / 2);
    Walker seed = Walker(center, walkerSize);
    tree.push_front(seed);
    for (int i = 0; i < walkerNum; i++) {
        ofVec3f newPos = onRadius();
        Walker newWalker = Walker(newPos, walkerSize);
        walkers.push_front(newWalker);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < walkerNum; i++) {
        for (int t = 0; t <= steps; t++) {
            walkers[i].walk();
            if (walkers[i].isCloseTo(tree) != -1) {
                tree.push_front(walkers[i]);
                walkerSize *= decrease;
                walkers[i].pos = onRadius();
                walkers[i].size = walkerSize;
            }
        }
        walkers[i].pos = onRadius();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    cam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2, -ofGetHeight() / 2);
    //--------------------------
    ofSetColor(ofColor::white);
    for (int i = 0; i < tree.size(); i++) {
        ofDrawSphere(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z, tree[i].size);
        /* 
        if (i + 1 < tree.size()) {
            ofDrawLine(
                tree[i].pos.x, tree[i].pos.y, tree[i].pos.z,
                tree[i + 1].pos.x, tree[i + 1].pos.y, tree[i + 1].pos.z);
        }
         */
    }

    ofSetColor(ofColor::blue);
    for (int i = 0; i < walkers.size(); i++) {
        ofDrawSphere(walkers[i].pos.x, walkers[i].pos.y, walkers[i].pos.z,
                     walkers[i].size);
    }
    //--------------------------
    ofPopMatrix();
    cam.end();
}
