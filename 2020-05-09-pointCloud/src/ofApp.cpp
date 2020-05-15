#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::initDatGui() {
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

    cameraXAngle.set("Cam. X rot.", 90, 0, 360);
    cameraXPos.set("Cam. X pos.", 0, -5.0f, 5.0f);
    displaceRadius.set("Displace r.", 10, 5.0f, 50.0f);
    noiseScale.set("Noise scale", 5, 1, 15);
    cameraYStart.set("Cam. start", -50, -100, 0);
    cameraYEnd.set("Cam. end", 50, 0, 100);
    bloomIntensity.set("Bloom strenght.", 100.0f, 10.0f, 300.0f);

    gui->addLabel("Camera settings");
    gui->addToggle("Ext. camera", false);
    gui->addSlider(cameraXAngle);
    gui->addSlider(cameraXPos);
    gui->addSlider(displaceRadius);
    gui->addSlider(noiseScale);
    gui->addLabel("Traveling length");
    gui->addSlider(cameraYStart);
    gui->addSlider(cameraYEnd);
    gui->addLabel("Material");
    gui->addSlider(bloomIntensity);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onToggleEvent(this, &ofApp::onToggleEvent);
}
//--------------------------------------------------------------
string ofApp::getPLYmodel(int modelId) {
    string models[10] = {
        "Almagro_Potree_Merged.ply",
        "arafo.ply",
        "auckland-offset.ply",
        "etna_ao.ply",
        "flinder-street.ply",
        "fused-pruned-tightened-5m-pts.ply",
        "GT_Madame1_2.ply",
        "kozushima-001-fused-pruned-tightened-10m-pts.ply",
        "ny-carlsberg-glyptotek-pointcloud.ply",
        "rc-renningsasen-asfaltverk.ply"};

    if (modelId >= 0 && modelId <= sizeof(models) - 1) {
        return models[modelId];
    } else {
        return models[0];
    }
}
//--------------------------------------------------------------
void ofApp::setup() {
    animFrame = 300;
    debug = false;

    ofSetFrameRate(30);
    ofSetVerticalSync(true);

    ofEnableDepthTest();
    ofSetGlobalAmbientColor(ofColor(255, 255, 255));
    ofSetSmoothLighting(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_POINT_SMOOTH);
    //glPointSize(2);

    mesh.load("plys/" + getPLYmodel(9));
    shader.load("shadersGL3/shader");

    camCollider.setPosition(0, 0, 0);
    camCollider.setRadius(displaceRadius);
    ofQuaternion travelingAngleStart;
    travelingAngleStart.makeRotate(90, 1, 0, 0);
    camCollider.setOrientation(travelingAngleStart);
    camera.setParent(camCollider);
    camera.setPosition(ofVec3f(0, 0, displaceRadius));
    initDatGui();

    debugCam.setVFlip(true);
    debugCam.setDistance(35);
}
//--------------------------------------------------------------
void ofApp::cameraMove() {
    // Get new param when animation finished
    if (ofGetFrameNum() % animFrame == 0) {
        camStartPos = ofVec3f(0, cameraYStart, 0);
        camTargetPos = ofVec3f(0, cameraYEnd, 0);
    }
    float tweenvalue = 1.f * (ofGetFrameNum() % animFrame) / animFrame;
    /* 
    ofVec3f startPos;
    ofVec3f targetPos;
    ofQuaternion startQuat;
    ofQuaternion targetQuat;
    startQuat.makeRotate(90, 1, 0, 0);
    targetQuat.makeRotate(90, 1, 0, 0);

    startPos.set(0, -25, 0);
    targetPos.set(0, 35, 0);
    ofQuaternion tweenedCameraQuaternion;
    tweenedCameraQuaternion.slerp(tweenvalue, startQuat, targetQuat);
    camera.setOrientation(tweenedCameraQuaternion);
    */

    ofVec3f lerpPos;
    lerpPos.x = ofLerp(camStartPos.x, camTargetPos.x, tweenvalue);
    lerpPos.y = ofLerp(camStartPos.y, camTargetPos.y, tweenvalue);
    lerpPos.z = ofLerp(camStartPos.z, camTargetPos.z, tweenvalue);
    camCollider.setPosition(lerpPos);
}
//--------------------------------------------------------------
void ofApp::update() {
    cameraMove();
    //pointCloudErode();
}
//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(20, 20, 20);
    ofEnableDepthTest();

    if (debug) {
        debugCam.begin();
    } else {
        camera.begin();
    }
    /* 
    if (debug) {
        camCollider.drawWireframe();
    } */

    shader.begin();
    shader.setUniform4f(
        "cameraPos",
        camCollider.getPosition().x,
        camCollider.getPosition().y,
        camCollider.getPosition().z,
        0);
    shader.setUniform1f("displaceRadius", displaceRadius);
    shader.setUniform1f("noiseScale", noiseScale);
    shader.setUniform2f(
        "resolution",
        ofDefaultVec2(ofGetWidth(), ofGetHeight()));
    shader.setUniform1f("bloomIntensity", bloomIntensity);
    shader.setUniform1f("time", ofGetElapsedTimef());
    mesh.drawVertices();

    shader.end();

    if (debug) {
        debugCam.end();
    } else {
        camera.end();
    }

    // Fix black ofxDatGui
    // https://github.com/braitsch/ofxDatGui/issues/111#issuecomment-264457723
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::debugAxis() {
    ofSetHexColor(0xff0000);
    ofFill();
    ofDrawLine(0, 0, 0, 1, 0, 0);

    ofSetHexColor(0x00ff00);
    ofFill();
    ofDrawLine(0, 0, 0, 0, 1, 0);

    ofSetHexColor(0x0000ff);
    ofFill();
    ofDrawLine(0, 0, 0, 0, 0, 1);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // if user press c key
    if (key == 99) {
    } else {
        std::cout << "unaffected key (" << key << ")" << endl;
    }
}

//--------------------------------------------------------------

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    // prevent move of ofEasyCam
    //debugCam.disableMouseInput();

    string param = e.target->getName();

    if (param == "Cam. X rot.") {
        ofQuaternion travelingAngleStart;
        travelingAngleStart.makeRotate(cameraXAngle, 1, 0, 0);
        camCollider.setOrientation(travelingAngleStart);
    }
    if (param == "Cam. X pos.") {
        camStartPos.x += cameraXPos;
        camTargetPos.x += cameraXPos;
    }
    if (param == "Displace r.") {
        camCollider.setRadius(displaceRadius);
        camera.setPosition(ofVec3f(0, 0, displaceRadius));
    }

    //debugCam.enableMouseInput();
}
//--------------------------------------------------------------
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
    if (e.target->is("Ext. camera")) {
        debug = !debug;
    }
}
