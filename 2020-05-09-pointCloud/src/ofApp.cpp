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

    gui->addLabel("Camera settings");
    gui->addToggle("Ext. camera", false);
    gui->addSlider(cameraXAngle);
    gui->addSlider(cameraXPos);
    gui->addSlider(displaceRadius);
    gui->addSlider(noiseScale);
    gui->addLabel("Traveling length");
    gui->addSlider(cameraYStart);
    gui->addSlider(cameraYEnd);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    gui->onToggleEvent(this, &ofApp::onToggleEvent);
}
//--------------------------------------------------------------
void ofApp::setup() {
    animFrame = 300;
    debug = false;

    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    mesh.load("plys/flinder-street.ply");
    shader.load("shadersGL3/shader");

    camCollider.setPosition(0, 0, 0);
    camCollider.setRadius(displaceRadius);
    ofQuaternion travelingAngleStart;
    travelingAngleStart.makeRotate(90, 1, 0, 0);
    camCollider.setOrientation(travelingAngleStart);
    camera.setParent(camCollider);
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
    ofBackground(0, 0, 0);
    ofEnableDepthTest();

    if (debug) {
        debugCam.begin();
    } else {
        camera.begin();
    }

    if (debug) {
        ofSetColor(200, 200, 200, 50);
        camCollider.drawWireframe();
    }

    shader.begin();
    shader.setUniform4f(
        "cameraPos",
        camCollider.getPosition().x,
        camCollider.getPosition().y,
        camCollider.getPosition().z,
        0);
    shader.setUniform1f("displaceRadius", displaceRadius);
    shader.setUniform1f("noiseScale", noiseScale);
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
    debugCam.disableMouseInput();

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
    }
}
//--------------------------------------------------------------
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
    if (e.target->is("Ext. camera")) {
        debug = !debug;
    }
}
