#include "ofApp.h"
//--------------------------------------------------------------
ofVec3f ofApp::randomPos() {
    return ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()),
                   ofRandom(ofGetHeight()));
}
//--------------------------------------------------------------
ofVec3f ofApp::onEmitter() {
    ofVec3f center = ofVec3f(0, 0, 0);
    float theta = ofRandom(1) * glm::pi<float>() * 2;
    float phi = glm::acos(1 - ofRandom(1) * 20);

    center.x += glm::sin(phi) * glm::cos(theta) * emitterDistance;
    center.y += glm::sin(phi) * glm::sin(theta) * emitterDistance;
    center.z += glm::cos(phi) * emitterDistance;

    return center;
}
//--------------------------------------------------------------
ofColor ofApp::indexColor(int index) {
    float cursor = ofMap(index, 0, tree.size(), 0, 1);
    ofColor startColor = ofColor::steelBlue;
    ofColor endColor = ofColor::tomato;
    return startColor.getLerped(endColor, cursor);
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

                    walkerSize *= decreaseWalkerSize;
                    emitterDistance *= increaseEmitterDistance;
                    walkers[i].pos = onEmitter();
                    walkers[i].size = walkerSize;
                }
            }
            walkers[i].pos = onEmitter();
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
        ofPushStyle();
        ofSetColor(indexColor(i));
        ofDrawIcoSphere(
            ofPoint(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z),
            tree[i].size);
        ofPopStyle();
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
    decreaseWalkerSize.addListener(this, &ofApp::decreaseValueChanged);
    initEmitterDistance.addListener(this, &ofApp::emitterDistanceChanged);
    increaseEmitterDistance.addListener(this, &ofApp::increaseValueChanged);
    treeSize.addListener(this, &ofApp::treeSizeChanged);
    gui.setup("PARAMETERS");

    gui.add(walkerNum.set(
        "Walker moved", 50, 100, 500));
    gui.add(steps.set(
        "Move step", 150, 0, 1000));
    gui.add(initWalkerSize.set(
        "W. init size", 32, 24, 76));
    gui.add(decreaseWalkerSize.set(
        "Decrease w. size", 1, 0.7, 1));
    gui.add(initEmitterDistance.set(
        "Emitter dist.", 480, 320, 720));
    gui.add(increaseEmitterDistance.set(
        "Increase emit. dist.", 1.05, 1, 1.25));
    gui.add(treeSize.set(
        "Total w.", 500, 500, 10000));
}
//--------------------------------------------------------------
void ofApp::init() {
    /* erase previous computed element */
    tree.clear();
    walkers.clear();
    walkerSize = initWalkerSize;
    emitterDistance = initEmitterDistance;
    ofVec3f center = ofVec3f(0, 0, 0);
    Walker seed = Walker(center, walkerSize);
    tree.push_front(seed);

    for (int i = 0; i < walkerNum; i++) {
        ofVec3f newPos = onEmitter();
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
void ofApp::decreaseValueChanged(float& decreaseWalkerSize) {
    init();
}
//--------------------------------------------------------------
void ofApp::emitterDistanceChanged(float& initEmitterDistance) {
    init();
}
//--------------------------------------------------------------
void ofApp::increaseValueChanged(float& increaseEmitterDistance) {
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