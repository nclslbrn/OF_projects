#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

  ofBackground(0);
  ofSetFrameRate(30);
  ofSetVerticalSync(true);
  ofEnableDepthTest();
  ofEnableAlphaBlending();
  ofEnableSmoothing();
  ofSetGlobalAmbientColor(ofColor(0, 0, 0));
  ofSetSmoothLighting(true);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  model.loadModel("ArnoldSchwarzeneggerBust.OBJ");
  model.setPosition(0, 0, 0);
  model.setRotation(0, 180, 0, 0, 1);
  model.setRotation(1, -20, 1, 0, 0);

  zRot = 0;

  point.setDiffuseColor(ofColor(255.0, 120.0, 120.0));
  point.setPointLight();
}

//--------------------------------------------------------------
void ofApp::update() { zRot += 0.05; }

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(0, 0, 0, 0);
  cam.begin();
  ofSetColor(200, 200, 200, 255);
  point.setPosition(0, 0, (float)ofGetHeight() * 0.75);
  point.enable();
  model.setRotation(2, zRot, 0, 1, 0);
  // model.draw(OF_MESH_WIREFRAME);
  // model.draw(OF_MESH_POINTS);
  model.draw(OF_MESH_FILL);
  point.disable();
  cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
