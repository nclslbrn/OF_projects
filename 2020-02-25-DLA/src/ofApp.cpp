#include "ofApp.h"
//--------------------------------------------------------------
ofVec3f ofApp::randomPos() {
  return ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()),
                 ofRandom(ofGetHeight()));
}
//--------------------------------------------------------------
ofVec3f ofApp::onRadius() {
  ofVec3f center = ofVec3f(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()/2);
  float theta = ofRandom(1) * glm::pi<float>() * 2;
  float phi = glm::acos( 1 - ofRandom(1) * 20);

  center.x = glm::sin(phi) * glm::cos(theta) * radius;
  center.y = glm::sin(phi) * glm::sin(theta) * radius;
  center.z = glm::cos(phi) * radius;

  return center;
}
//--------------------------------------------------------------
void ofApp::setup() {
  radius = ofGetHeight()/4;
  steps = 12;
  walkerNum = 15;
  walkerSize = 5;
  decrease = 0.995;

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
      if (walkers[i].isCloseTo(walkers) > 0) {
        walkerSize *= decrease;
        tree.push_front(walkers[i]);
        walkers[i].pos = onRadius();
        walkers[i].size = walkerSize;
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);
  cam.begin();
  ofPushMatrix();
  ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2, -ofGetHeight() / 1.5);
  //--------------------------
  ofSetColor(ofColor::white);
  for (int i = 0; i < tree.size(); i++) {
    ofDrawSphere(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z, tree[i].size);
  }
  /*
    ofSetColor(ofColor::tomato);
    for (int i = 0; i < walkers.size(); i++) {
      ofDrawSphere(walkers[i].pos.x, walkers[i].pos.y, walkers[i].pos.z,
                   walkers[i].size);
    } */
  //--------------------------
  ofPopMatrix();
  cam.end();
}
