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

    ofSetFrameRate(25);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetGlobalAmbientColor(ofColor(0, 0, 0));
    ofSetSmoothLighting(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    radius = min(ofGetWidth(), ofGetHeight()) / 3;
    steps = 500;
    walkerNum = 500;
    walkerSize = 10;
    decrease = 0.93;
    zRot = 0;
    ofVec3f center = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, ofGetHeight() / 2);
    Walker seed = Walker(center, walkerSize);
    tree.push_front(seed);
    for (int i = 0; i < walkerNum; i++) {
        ofVec3f newPos = onRadius();
        Walker newWalker = Walker(newPos, walkerSize);
        walkers.push_front(newWalker);
    }
    point.setDiffuseColor(ofColor(200.0, 255.0, 255.0));
    point.setPointLight();
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < walkerNum; i++) {
        for (int t = 0; t <= steps; t++) {
            walkers[i].walk();
            int treeID = walkers[i].isCloseTo(tree);

            if ( treeID != -1) {
                Branch newBranch = Branch(tree[treeID].pos, walkers[i].pos, walkerSize);
                branches.push_front(newBranch);
                tree.push_front(walkers[i]);

                walkerSize *= decrease;
                walkers[i].pos = onRadius();
                walkers[i].size = walkerSize;
            }
        }
        walkers[i].pos = onRadius();
    }
    zRot+= 0.1;
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    cam.begin();
    point.setPosition(0, 0, (float)ofGetHeight() * 0.75);
    point.enable();
    ofPushMatrix();
    //ofRotateZ(zRot);
    ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2, -ofGetHeight() / 2);
    //--------------------------
    ofSetColor(ofColor::white);
    for( int i = 0; i < branches.size(); i++ ) {
        branches[i].draw();
    }
    ofPopMatrix();
    point.disable();
    cam.end();
}
