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
    ofEnableLighting();
    ofEnableDepthTest();
    ofEnableSmoothing();
    ofSetSmoothLighting(true);
    ofSetGlobalAmbientColor(ofFloatColor(0, 0, 0));

    animFrame = 320;
    width = 200;
    height = 112;
    depth = 32;
    noiseRadius = 0.5;
    noiseScale = 0.035;
    mainCam.setPosition(0, 0, 80);

    //pointLight.setPointLight();
    pointLight.setup();
    pointLight.enable();
    pointLight.setPosition(16, 9, 340);
    pointLight.setDiffuseColor(ofColor(250, 235, 215));

    material.setAmbientColor(ofColor(0, 0, 0));
    material.setDiffuseColor(ofColor(0, 0, 0));
    material.setSpecularColor(ofColor(150, 150, 150));
    material.setShininess(30);

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
    setNormals(mainMesh);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    /* ofEnableLighting();
    pointLight.enable(); */
    mainCam.begin();
    material.begin();
    mainMesh.drawFaces();
    ofSetColor(0, 0, 0);
    mainMesh.drawVertices();
    mainMesh.drawWireframe();
    material.end();
    mainCam.end();
}
//--------------------------------------------------------------

void ofApp::setNormals(ofMesh &mesh) {
    mesh.clearNormals();

    //The number of the vertices
    int nV = mesh.getNumVertices();

    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;

    vector<ofPoint> norm(nV);  //Array for the normals

    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t = 0; t < nT; t++) {
        //Get indices of the triangle t
        int i1 = mesh.getIndex(3 * t);
        int i2 = mesh.getIndex(3 * t + 1);
        int i3 = mesh.getIndex(3 * t + 2);

        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex(i1);
        const ofPoint &v2 = mesh.getVertex(i2);
        const ofPoint &v3 = mesh.getVertex(i3);

        //Compute the triangle's normal
        ofPoint dir = ((v2 - v1).crossed(v3 - v1)).normalized();

        //Accumulate it to norm array for i1, i2, i3
        norm[i1] += dir;
        norm[i2] += dir;
        norm[i3] += dir;
    }

    //Normalize the normal's length
    for (int i = 0; i < nV; i++) {
        norm[i].normalize();
        mesh.addNormal(norm[i]);
    }
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
