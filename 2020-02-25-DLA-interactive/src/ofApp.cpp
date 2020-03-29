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
    ofColor startColor = ofColor(41, 128, 185, 255);
    ofColor endColor = ofColor(192, 57, 43, 255);
    return startColor.getLerped(endColor, cursor);
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetGlobalAmbientColor(ofColor(150, 150, 150));
    ofSetSmoothLighting(true);
    ofDisableArbTex();
    ofEnableDepthTest();

    initDatGui();
    init();

    zRot = 0;
    xRot = 0;
    zoom = ofGetHeight() / 4;
    mouseScrollSensivity = 16;

    cam.setupPerspective();
    cam.setPosition(0, 0, ofGetHeight());
    cam.lookAt(ofVec3f(0, 0, 0));

    pointLight.setPointLight();
    /* pointLight.setAmbientColor(ofFloatColor(.5, .5, .5));
    pointLight.setDiffuseColor(ofFloatColor(.6, .6, .6));*/
    pointLight.setSpecularColor(ofFloatColor(255, 255, 255));
    pointLight.setPosition(0, -ofGetHeight(), ofGetHeight() / 2);

    treeMat.setShininess(15);
}

//--------------------------------------------------------------
void ofApp::update() {
    /* walkerNumSlider->update();
    stepsSlider->update();
    initWalkerSizeSlider->update();
    decreaseWalkerSizeSlider->update();
    initEmitterDistanceSlider->update();
    increaseEmitterDistanceSlider->update();
    treeSizeSlider->update(); */

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

    cam.begin();
    ofEnableLighting();
    pointLight.enable();

    ofPushMatrix();
    ofTranslate(0, 0, zoom);
    ofRotateX(xRot);
    ofRotateZ(zRot);

    for (int i = 0; i < tree.size(); i++) {
        ofPushStyle();
        treeMat.setDiffuseColor(indexColor(i));
        treeMat.begin();
        ofDrawIcoSphere(
            ofPoint(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z),
            tree[i].size);
        treeMat.end();
        ofPopStyle();
    }

    ofPopMatrix();
    pointLight.disable();

    ofDisableLighting();
    cam.end();

    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofDrawBitmapStringHighlight(
        ofToString(tree.size()) + "/" + ofToString(treeSize),
        ofVec3f(0, ofGetHeight(), 0));
}
//--------------------------------------------------------------
void ofApp::initGui() {
    /* ofxGui first setup
    
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
        "Emitter dist.", 376, 320, 720));
    gui.add(increaseEmitterDistance.set(
        "Increase emit. dist.", 1.0075, 1, 1.25));
    gui.add(treeSize.set(
        "Total w.", 500, 500, 10000));
    */
}
//--------------------------------------------------------------
void ofApp::initDatGui() {
    walkerNum.set("walkerNum", 50, 100, 500);
    steps.set("steps", 150, 0, 1000);
    initWalkerSize.set("initWalkerSize", 32, 24, 76);
    decreaseWalkerSize.set("decreaseWalkerSize", 1, 0.7, 1);
    initEmitterDistance.set("initEmitterDistance", 376, 320, 720);
    increaseEmitterDistance.set("increaseEmitterDistance", 1.0075, 1, 1.25);
    treeSize.set("treeSize", 500, 500, 10000);

    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->addLabel("DLA parameters");
    gui->addSlider(walkerNum);
    gui->addSlider(steps);
    gui->addSlider(initWalkerSize);
    gui->addSlider(decreaseWalkerSize);
    gui->addSlider(initEmitterDistance);
    gui->addSlider(increaseEmitterDistance);
    gui->addSlider(treeSize);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
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

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    string param = e.target->getName();
    if (
        param == "walkerNum" ||
        param == "walkerSize" ||
        param == "decreaseWalkerSize" ||
        param == "initEmitterDistance" ||
        param == "increaseEmitterDistance") {
        init();
    }
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    zRot = ofMap(x, 0, ofGetWidth(), 0, 360);
    xRot = ofMap(y, 0, ofGetHeight(), 0, 360);
}
//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
    zoom += (scrollY * mouseScrollSensivity);
}