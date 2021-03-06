#include "ofApp.h"

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

    increaseEmitterDistance = 1.001;
    decreaseWalkerSize = 1;

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
    isExporting = false;
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

    treeMat.setShininess(0.1);
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
    if (!isExporting && !isExported) {
        getInfoString();
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
        ofDrawBox(
            ofPoint(tree[i].pos.x, tree[i].pos.y, tree[i].pos.z),
            tree[i].size);
        treeMat.end();
    }

    ofPopMatrix();

    cam.end();

    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    ofDrawBitmapStringHighlight(
        ofToString(tree.size()) + "/" + ofToString(treeSize) + factorInfo,
        ofVec3f(0, ofGetHeight() - 5, 0));

    // Fix black ofxDatGui
    // https://github.com/braitsch/ofxDatGui/issues/111#issuecomment-264457723
    ofDisableDepthTest();
}
//--------------------------------------------------------------
void ofApp::initDatGui() {
    walkerNum.set("Living count", 200, 100, 500);
    steps.set("Move step", 150, 15, 200);
    initWalkerSize.set("Initial size", 24, 12, 76);
    decreaseParam.set("Shrink size", 100, 0, 100);  // 1 % from 0.9 to 1

    initEmitterDistance.set("Emitter distance", 100, 50, 500);
    increaseParam.set("Grow emit. dist.", 100, 0, 100);  //  1.01 % from 1 to 1.001
    treeSize.set("Desired size", 500, 500, 10000);

    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    gui->setTheme(new ofxDatGuiThemeSmoke());
    gui->addHeader("DLA parameters");
    gui->addLabel("Walker options");
    gui->addSlider(walkerNum);
    gui->addSlider(steps);
    gui->addSlider(initWalkerSize);
    gui->addSlider(decreaseParam);
    gui->addLabel("Tree options");
    gui->addSlider(initEmitterDistance);
    gui->addSlider(increaseParam);
    gui->addSlider(treeSize);
    gui->addButton("EXPORT DLA TO CSV");
    gui->addButton("EXPORT DLA TO PLY");
    gui->addFooter();
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onButtonEvent(this, &ofApp::onButtonEvent);
}
//--------------------------------------------------------------
void ofApp::init() {
    /* change app state */
    isExporting = false;
    isExported = false;
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

    /* Get parameters value to print them */
    factorInfo = " | Tree initialized";
}
//--------------------------------------------------------------
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    string param = e.target->getName();
    if (
        param == "Living count" ||
        param == "Move step" ||
        param == "Initial size" ||
        param == "Shrink w. size" ||
        param == "Emitter distance" ||
        param == "Grow emit. dist.") {
        if (param == "Shrink w. size") {
            std::printf("Shrink %f\n", decreaseParam);
            decreaseWalkerSize = ofMap(decreaseParam, 0, 100, 0.9, 1);
        }
        if (param == "Grow emit. dist.") {
            std::printf("Grow %f\n", increaseParam);
            increaseEmitterDistance = ofMap(increaseParam, 0, 100, 1, 1.001);
        }

        init();
    }
    getInfoString();
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
    if (e.target->getLabel() == "EXPORT DLA TO CSV") {
        exportTreeInCSVformat();
    }
    if (e.target->getLabel() == "EXPORT DLA TO PLY") {
        exportTreeInPLYformat();
    }
}
//--------------------------------------------------------------
void ofApp::getInfoString() {
    if (tree.size() < treeSize) {
        string shrinkWalkerInfo = "Shrink walker size: " + ofToString(decreaseWalkerSize);
        string growEmitterInfo = "Grow emitter distance: " + ofToString(increaseEmitterDistance);

        factorInfo = " | " + shrinkWalkerInfo + " | " + growEmitterInfo;
    } else {
        factorInfo = " | Tree computation completed, ready to export.";
    }
}
//--------------------------------------------------------------
void ofApp::exportTreeInCSVformat() {
    isExporting = true;
    factorInfo = " | Exporting tree into CSV ...............................................";

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
    isExported = true;
    factorInfo = " | Tree was exported in bin/data/output/" + fileName + ".";
}
//--------------------------------------------------------------
void ofApp::exportTreeInPLYformat() {
    isExporting = true;
    factorInfo = " | Exporting tree into PLY ...............................................";
    string path = "output/";
    string fileName = "DLA-tree-" + ofGetTimestampString() + ".ply";
    ofFile ply(path + fileName, ofFile::WriteOnly);
    ply << "ply" << endl;
    ply << "format ascii 1.0" << endl;
    ply << "comment VCGLIB generated" << endl;
    ply << "element vertex " + ofToString(tree.size()) << endl;
    ply << "property float x" << endl;
    ply << "property float y" << endl;
    ply << "property float z" << endl;
    ply << "property uchar red" << endl;
    ply << "property uchar green" << endl;
    ply << "property uchar blue" << endl;
    ply << "property uchar alpha" << endl;
    ply << "element face 0" << endl;
    ply << "property list uchar int vertex_indices" << endl;
    ply << "end_header" << endl;

    for (int i = 0; i < tree.size(); i++) {
        ofColor vCol = indexColor(i);
        ply << ofToString(tree[i].pos.x) + " " +
                   ofToString(tree[i].pos.y) + " " +
                   ofToString(tree[i].pos.z) + " " +
                   ofToString((int)vCol.r) + " " +
                   ofToString((int)vCol.g) + " " +
                   ofToString((int)vCol.b) + " 255"
            << endl;
    }
    ply.close();
    isExporting = false;
    isExported = true;
    factorInfo = " | Tree was exported in bin/data/output/" + fileName + ".";
    launchBlender(fileName);
}
//--------------------------------------------------------------
void ofApp::launchBlender(string fileName) {
    string binDataPath = ofFilePath::getCurrentExeDir();

    string blenderCommand =
        "blender " + binDataPath + "data/blender-script/almost-empty.blend " +
        "--python " + binDataPath + "data/blender-script/import-dla.py " +
        "-- " + binDataPath + "/data/output/" + fileName + " " + ofToString(treeSize);

    ofSystem(blenderCommand.c_str());
}