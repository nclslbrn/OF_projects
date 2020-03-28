#include "ofApp.h"
//--------------------------------------------------------------
ofVec3f ofApp::randomPos() {
    return ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()),
                   ofRandom(ofGetHeight()));
}
//--------------------------------------------------------------
ofVec3f ofApp::onRadius() {
    ofVec3f center = ofVec3f(0, 0, 0);
    float theta = ofRandom(1) * glm::pi<float>() * 2;
    float phi = glm::acos(1 - ofRandom(1) * 20);

    center.x += glm::sin(phi) * glm::cos(theta) * radius;
    center.y += glm::sin(phi) * glm::sin(theta) * radius;
    center.z += glm::cos(phi) * radius;

    return center;
}
//--------------------------------------------------------------
ofColor ofApp::indexColor(float indexPercent) {
    ofColor startColor = ofColor::steelBlue;
    ofColor endColor = ofColor::tomato;
    return startColor.getLerped(endColor, indexPercent);
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    //ofSetGlobalAmbientColor(ofColor(150, 150, 150));
    ofSetSmoothLighting(true);
    //ofDisableArbTex();
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    initGui();
    init();

    zRot = 0;
    xRot = 0;
    zoom = 0;
    mouseScrollSensivity = 16;

    pointLight.setPointLight();
    pointLight.setDiffuseColor(ofFloatColor(.85, .85, .85));
    pointLight.setSpecularColor(ofFloatColor(1, 1, 1));
    pointLight.setPosition(0, 0, ofGetHeight() * 2);

    ofSetSphereResolution(24);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (tree.size() < treeSize) {
        for (int i = 0; i < walkerNum; i++) {
            for (int t = 0; t <= steps; t++) {
                walkers[i].walk();
                int treeID = walkers[i].isCloseTo(tree);

                if (treeID != -1) {
                    tree.push_front(walkers[i]);

                    walkerSize *= decrease;
                    walkers[i].pos = onRadius();
                    walkers[i].size = walkerSize;
                }
            }
            walkers[i].pos = onRadius();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    ofDisableDepthTest();
    gui.draw();
    ofEnableDepthTest();
    ofEnableLighting();
    pointLight.enable();

    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, zoom);
    ofRotateX(xRot);
    ofRotateZ(zRot);

    for (int i = 0; i < tree.size(); i++) {
        //ofPushStyle();
        ofFill();
        ofSetColor(indexColor(i / tree.size()));
        ofDrawIcoSphere(
            ofPoint(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z),
            tree[i].size);
        //ofPopStyle();
    }

    ofPopMatrix();
    pointLight.disable();
    ofDisableLighting();

    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofDrawBitmapStringHighlight(
        ofToString(tree.size()) + "/" + ofToString(treeSize),
        ofVec3f(0, ofGetHeight(), 0));
}
//--------------------------------------------------------------
void ofApp::initGui() {
    walkerNum.addListener(this, &ofApp::walkerNumChanged);
    steps.addListener(this, &ofApp::stepsValueChaged);
    initWalkerSize.addListener(this, &ofApp::initWalkersSizeChanged);
    decrease.addListener(this, &ofApp::decreaseValueChanged);
    radius.addListener(this, &ofApp::radiusValueChanged);
    treeSize.addListener(this, &ofApp::treeSizeChanged);
    gui.setup("PARAMETERS");

    gui.add(walkerNum.set("walkerNum", 50, 100, 500));
    gui.add(steps.set("steps", 150, 0, 1000));
    gui.add(initWalkerSize.set("initWalkerSize", 32, 24, 76));
    gui.add(decrease.set("decrease", 1, 0.7, 1));
    gui.add(radius.set("radius", 480, 320, 720));
    gui.add(treeSize.set("treeSize", 500, 500, 10000));
}
//--------------------------------------------------------------
void ofApp::init() {
    /* erase previous computed element */
    tree.clear();
    walkers.clear();
    walkerSize = initWalkerSize;
    ofVec3f center = ofVec3f(0, 0, 0);
    Walker seed = Walker(center, walkerSize);
    tree.push_front(seed);

    for (int i = 0; i < walkerNum; i++) {
        ofVec3f newPos = onRadius();
        Walker newWalker = Walker(newPos, walkerSize);
        walkers.push_front(newWalker);
    }
}
//--------------------------------------------------------------
void ofApp::walkerNumChanged(int& walkerNum) {
    init();
}
//--------------------------------------------------------------
void ofApp::stepsValueChaged(int& steps) {
    //init();
}
//--------------------------------------------------------------
void ofApp::initWalkersSizeChanged(float& walkerSize) {
    init();
}
//--------------------------------------------------------------
void ofApp::decreaseValueChanged(float& decrease) {
    init();
}
//--------------------------------------------------------------
void ofApp::radiusValueChanged(float& radius) {
    init();
}
//--------------------------------------------------------------
void ofApp::treeSizeChanged(int& treeSize) {
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    xRot = ofMap(y, 0, ofGetHeight(), 0, 360);
    zRot = ofMap(x, 0, ofGetWidth(), 0, 360);
}
//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    zoom += (scrollY * mouseScrollSensivity);
}