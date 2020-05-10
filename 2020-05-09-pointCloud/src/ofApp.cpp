#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::initDatGui() {
    cameraXAngle.set("Cam. X rot.", 90, 0, 360);
    cameraXPos.set("Cam. X pos.", 0, -5.0f, 5.0f);
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->addLabel("Camera settings");
    gui->addSlider(cameraXAngle);
    gui->addSlider(cameraXPos);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
}
//--------------------------------------------------------------
void ofApp::setup() {
    animFrame = 300;
    cameraColliderSize = 5;
    debug = false;
    camStartPos = ofVec3f(0, -25, 0);
    camTargetPos = ofVec3f(0, 35, 0);

    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    streetModel.load("plys/GT_Madame1_2.ply");

    point.setDiffuseColor(ofColor(255.0, 120.0, 120.0));
    point.setPointLight();

    shader.load("shadersGL3/shader");

    camNode.setPosition(0, 0, 0);
    cameraCollider = ofMesh::box(
        cameraColliderSize,
        cameraColliderSize,
        cameraColliderSize);
    ofQuaternion travelingAngleStart;
    travelingAngleStart.makeRotate(90, 1, 0, 0);
    camNode.setOrientation(travelingAngleStart);
    camera.setParent(camNode);
    camNode.getMesh().append(cameraCollider);
    camNode.setPosition(camStartPos);

    initDatGui();
}

//--------------------------------------------------------------
void ofApp::update() {
    cameraMove();
    pointCloudErode();
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

    ofSetColor(200, 200, 200, 255);
    point.setGlobalPosition(0, 0, (float)ofGetHeight() * 0.75);
    point.enable();

    debugAxis();
    cameraCollider.drawWireframe();

    shader.begin();
    streetModel.drawVertices();

    shader.end();
    point.disable();

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
void ofApp::pointCloudErode() {
    /*     for (auto& model : streetModel) {
        for (auto& vertex : model.getVertices()) {
            vertex += ofVec3f(0, 0, 100 * (0.5 - ofRandomuf()));
        }
} */
}
//--------------------------------------------------------------
void ofApp::cameraMove() {
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
    camNode.setPosition(lerpPos);
}

//--------------------------------------------------------------
void ofApp::debugAxis() {
    ofSetHexColor(0xff0000);
    ofFill();
    ofDrawLine(0, 0, 0, 100, 0, 0);

    ofSetHexColor(0x00ff00);
    ofFill();
    ofDrawLine(0, 0, 0, 0, 100, 0);

    ofSetHexColor(0x0000ff);
    ofFill();
    ofDrawLine(0, 0, 0, 0, 0, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // if user press c key
    if (key == 99) {
        ofVec3f camPos = camNode.getPosition();
        float rotation[4];
        rotation[0] = camera.getOrientationQuat().x;
        rotation[1] = camera.getOrientationQuat().y;
        rotation[2] = camera.getOrientationQuat().z;
        rotation[3] = camera.getOrientationQuat().w;

        std::cout
            << "X:" << camPos.x
            << " Y:" << camPos.y
            << " Z:" << camPos.z << endl;

        std::cout
            << "rX:" << rotation[0]
            << " rY:" << rotation[1]
            << " rZ:" << rotation[2]
            << " rW:" << rotation[3] << endl;

    } else {
        std::cout << "unaffected key (" << key << ")" << endl;
    }
}

//--------------------------------------------------------------

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
    string param = e.target->getName();

    if (param == "Cam. X rot.") {
        ofQuaternion travelingAngleStart;
        travelingAngleStart.makeRotate(cameraXAngle, 1, 0, 0);
        camNode.setOrientation(travelingAngleStart);
    }
    if (param == "Cam. X pos.") {
        camStartPos.x += cameraXPos;
        camTargetPos.x += cameraXPos;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
