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
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    initDatGui();
    init();

    zRot = 0;
    xRot = 0;
    zoom = ofGetHeight() / 4;
    mouseScrollSensivity = 16;

    cam.setPosition(0, 0, ofGetHeight());
    cam.lookAt(ofVec3f(0, 0, 0));

    ofEnableLighting();
    // white light
    wLight.setPointLight();
    wLight.setDiffuseColor(ofFloatColor(1, 1, 1));
    wLight.setPosition(0, -ofGetHeight(), ofGetHeight() / 2);
    wLight.enable();

    // orange light
    oLight.setPointLight();
    oLight.setDiffuseColor(ofFloatColor(1, 0.75, 0));
    oLight.setPosition(-ofGetWidth(), -ofGetWidth() / 2, 0);
    oLight.enable();

    // blue light
    bLight.setPointLight();
    bLight.setDiffuseColor(ofFloatColor(0, 0, 1));
    bLight.setPosition(ofGetWidth(), ofGetWidth() / 2, 0);
    bLight.enable();

    treeMat.setShininess(128);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (tree.size() < treeSize && !isExporting) {
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
    ofBackgroundGradient(ofColor(50, 50, 50), ofColor::black);
    ofEnableDepthTest();

    cam.begin();

    ofPushMatrix();
    ofTranslate(0, 0, zoom);
    ofRotateX(xRot);
    ofRotateZ(zRot);

    for (int i = 0; i < tree.size(); i++) {
        treeMat.setDiffuseColor(indexColor(i));
        treeMat.begin();
        ofDrawIcoSphere(
            ofPoint(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z),
            tree[i].size);
        treeMat.end();
    }

    ofPopMatrix();

    cam.end();

    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofDrawBitmapStringHighlight(
        ofToString(tree.size()) + "/" + ofToString(treeSize),
        ofVec3f(0, ofGetHeight(), 0));

    // Fix black ofxDatGui
    // https://github.com/braitsch/ofxDatGui/issues/111#issuecomment-264457723
    ofDisableDepthTest();
}
//--------------------------------------------------------------

void ofApp::initDatGui() {
    walkerNum.set("walkerNum", 50, 100, 500);
    steps.set("steps", 150, 15, 200);
    initWalkerSize.set("initWalkerSize", 32, 24, 76);
    decreaseWalkerSize.set("decreaseWalkerSize", 0.99, 0.95, 1);
    initEmitterDistance.set("initEmitterDistance", 300, 220, 720);
    increaseEmitterDistance.set("increaseEmitterDistance", 1.0075, 1, 1.05);
    treeSize.set("treeSize", 500, 500, 10000);

    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->addLabel("DLA parameters");
    gui->addSlider(walkerNum);
    gui->addSlider(steps);
    gui->addSlider(initWalkerSize);
    gui->addSlider(decreaseWalkerSize);
    gui->addSlider(initEmitterDistance);
    gui->addSlider(increaseEmitterDistance);
    gui->addSlider(treeSize);
    gui->addButton("Export");
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
}
//--------------------------------------------------------------
void ofApp::init() {
    /* erase previous computed element */
    isExporting = false;
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
//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
    std::cout << "Button clicked" << endl;
    exportTree();
}
//--------------------------------------------------------------
void ofApp::exportTree() {
    isExporting = true;
    string path = "output/";
    string fileName = "DLA-tree-" + ofGetTimestampString() + ".csv";
    csv.createFile(path + fileName);

    for (int i = 0; i < tree.size(); i++) {
        ofxCsvRow& row = csv.getRow(i);
        row.setFloat(0, tree[i].size);
        row.setFloat(1, tree[i].pos.x);
        row.setFloat(2, tree[i].pos.y);
        row.setFloat(3, tree[i].pos.z);

        csv.addRow(row);
    }
    csv.save(path + fileName);
    isExporting = false;
    std::cout << "Exporting" << endl;
}