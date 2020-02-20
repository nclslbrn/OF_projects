#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
float ofApp::softPlus(float q, float p) {
  float qq = q + p;
  if (qq <= 0) {
    return 0;
  }
  if (qq >= 2 * p) {
    return qq - p;
  }
  return 1 / (4 * p) * qq * qq;
}
//--------------------------------------------------------------
void ofApp::setup() {
  animFrame = 320;
  width = 160;
  height = 90;
  depth = 32;
  noiseRadius = 0.5;
  noiseScale = 0.05;
  mainCam.setPosition(0, 0, 80);
  pointLight.setPointLight();
  pointLight.setPosition(0, ofGetHeight(), ofGetHeight());
  pointLight.setDiffuseColor(ofColor(250, 235, 215));
  ofSetFrameRate(50);
  ofBackground(0, 0, 0);

  ofEnableDepthTest();

  ofSetGlobalAmbientColor(ofColor(250, 235, 215));
  ofSetSmoothLighting(true);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      mainMesh.addVertex(ofPoint(x - width / 2, y - height / 2));

      if (y % 2 == 0 || x % 2 == 0) {
        mainMesh.addColor(ofFloatColor(0, 0, 0));
      } else {
        mainMesh.addColor(ofFloatColor(255, 255, 255));
      }
    }
  }
  for (int y = 0; y < height - 1; y++) {
    for (int x = 0; x < width - 1; x++) {
      mainMesh.addIndex(x + y * width);
      mainMesh.addIndex((x + 1) + y * width);
      mainMesh.addIndex(x + (y + 1) * width);

      mainMesh.addIndex((x + 1) + y * width);
      mainMesh.addIndex((x + 1) + (y + 1) * width);
      mainMesh.addIndex(x + (y + 1) * width);
    }
  }
}
//--------------------------------------------------------------
void ofApp::update() {
  int frameDrawn = (int)ofGetFrameNum() < animFrame
                       ? (int)ofGetFrameNum()
                       : (int)ofGetFrameNum() % animFrame;
  float t = ofMap(frameDrawn, 0, animFrame, 0, 1);

  int i = 0;

  for (int y = 0; y < height; y++) {

    float yDif = ofMap(y, y < height / 2 ? 0 : height / 2,
                       y < height / 2 ? height / 2 : height,
                       y < height / 2 ? 0 : 1, y < height / 2 ? 1 : 0);

    for (int x = 0; x < width; x++) {
      float xDif = ofMap(x, x < width / 2 ? 0 : width / 2,
                         x < width / 2 ? width / 2 : width,
                         x < width / 2 ? 0 : 1, x < width / 2 ? 1 : 0);

      float noiseValue =
          ofNoise(x * noiseScale, y * noiseScale,
                  noiseRadius * (glm::cos(t * (glm::pi<float>() * 2))),
                  noiseRadius * (glm::sin(t * (glm::pi<float>() * 2))));

      float scale = depth * pow((yDif + xDif) / 2, 2);
      ofVec3f newPosition = mainMesh.getVertex(i);
      newPosition.z = noiseValue * scale;
      mainMesh.setVertex(i, newPosition);
      i++;
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw() {
  pointLight.enable();
  mainCam.begin();
  // mainMesh.drawWireframe();
  // mainMesh.drawVertices();
  mainMesh.drawFaces();
  mainCam.end();
  pointLight.disable();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
  /*
  noiseScale = ofMap(x, 0, ofGetWidth(), 0, 1);
  noiseRadius = ofMap(y, 0, ofGetHeight(), 0, 12);
  std::cout << "scale: " << ofToString(noiseScale) << " radius:" <<
  ofToString(noiseRadius) << endl;

*/
}

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
