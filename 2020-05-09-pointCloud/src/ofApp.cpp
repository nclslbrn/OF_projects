#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    animFrame = 1000;
    cameraColliderSize = 15;

    ofSetFrameRate(30);
    ofEnableDepthTest();

    streetModel.load("plys/GT_Madame1_2.ply");
    point.setDiffuseColor(ofColor(255.0, 120.0, 120.0));
    point.setPointLight();

    shader.load("shadersGL3/shader");
    cameraCollider = ofMesh::box(
        cameraColliderSize,
        cameraColliderSize,
        cameraColliderSize);
    camera.setParent(camGroup);
    camGroup.getMesh().append(cameraCollider);
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
    camera.begin();
    ofSetColor(200, 200, 200, 255);
    point.setPosition(0, 0, (float)ofGetHeight() * 0.75);
    point.enable();

    debugAxis();
    cameraCollider.drawWireframe();

    shader.begin();
    streetModel.drawVertices();
    shader.end();
    point.disable();
    camera.end();
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

    ofQuaternion startQuat;
    ofQuaternion targetQuat;
    ofVec3f startPos;
    ofVec3f targetPos;
    startQuat.makeRotate(90, 1, 0, 0);
    targetQuat.makeRotate(90, 1, 0, 0);

    startPos.set(0, -50, 0);
    targetPos.set(0, 25, 0);

    ofQuaternion tweenedCameraQuaternion;
    tweenedCameraQuaternion.slerp(tweenvalue, startQuat, targetQuat);

    ofVec3f lerpPos;
    lerpPos.x = ofLerp(startPos.x, targetPos.x, tweenvalue);
    lerpPos.y = ofLerp(startPos.y, targetPos.y, tweenvalue);
    lerpPos.z = ofLerp(startPos.z, targetPos.z, tweenvalue);

    camera.setOrientation(tweenedCameraQuaternion);
    camGroup.setPosition(lerpPos);
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
        ofVec3f camPos = camera.getPosition();
        float rotation[4];
        rotation[0] = camera.getOrientationQuat().x;
        rotation[1] = camera.getOrientationQuat().y;
        rotation[2] = camera.getOrientationQuat().z;
        rotation[3] = camera.getOrientationQuat().w;

        std::cout << "X:" << camPos.x << " Y:" << camPos.y << " Z:" << camPos.z << endl;
        std::cout << "rX:" << rotation[0] << " rY:" << rotation[1] << " rZ:" << rotation[2] << " rW:" << rotation[3] << endl;
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
