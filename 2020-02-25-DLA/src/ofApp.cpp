#include "ofApp.h"
//--------------------------------------------------------------
ofVec3f ofApp::randomPos() {
    return ofVec3f(
        ofRandom(ofGetWidth()),
        ofRandom(ofGetHeight()),
        ofRandom(ofGetHeight()));
}
//--------------------------------------------------------------
void ofApp::setup() {
    walkerNum = 250;
    initSize = 3;
    decrease = 0.995;

    for (int i = 0; i < walkerNum; i++) {
        ofVec3f newPos = randomPos();
        Walker newWalker = Walker(newPos, initSize);
        walkers.push_front(newWalker);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < walkerNum; i++) {
        walkers[i].walk();
        if (walkers[i].isCloseTo(walkers) > 0) {
            initSize *= decrease;
            tree.push_front(walkers[i]);
            walkers[i].pos = randomPos();
            walkers[i].size = initSize;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    cam.begin();

    ofSetColor(ofColor::white);
    for (int i = 0; i < tree.size(); i++) {
        ofDrawSphere(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z, tree[i].size);
    }

    ofSetColor(ofColor::tomato);
    for (int i = 0; i < walkers.size(); i++) {
        ofDrawSphere(walkers[i].pos.x, walkers[i].pos.y, walkers[i].pos.z, walkers[i].size);
    }

    cam.end();
}
