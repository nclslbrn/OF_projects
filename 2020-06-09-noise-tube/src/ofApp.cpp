#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetGlobalAmbientColor(ofColor(150, 150, 150));
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 156);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

    arcRadius = ofGetWidth() / 2.0f;
    radius = 100.0f;
    outerSteps = floor(glm::half_pi<double>() / res);
    innerSteps = floor(glm::two_pi<double>() / res);
    cubeSize = (glm::pi<float>() * 2 * radius) / innerSteps;

    cam.setAutoDistance(false);
    ofxLoadCamera(cam, camPresetFile);
    light.setPointLight();
    light.setPosition(ofGetWidth(), ofGetHeight(), ofGetHeight());
    light.enable();

    std::cout << "tube diameter: " << glm::pi<float>() * 2 * radius << endl;
    std::cout << "cubeSize: " << cubeSize << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor(50, 50, 50), ofColor::black);
    ofEnableDepthTest();
    cam.begin();
    float currRadius = radius;
    float currCubeSize = cubeSize;

    for (int i = 0; i >= -outerSteps; i--) {
        double theta = res * i;
        ofVec3f v1 = ofVec3f(arcRadius * glm::cos(theta), ofGetHeight() + arcRadius * glm::sin(theta), 0);
        for (int j = 0; j <= innerSteps; j++) {
            double phi = res * j;
            ofVec3f v2 = ofVec3f(
                v1.x + (currRadius * glm::sin(phi) * glm::cos(theta)),
                v1.y + (currRadius * glm::sin(phi) * glm::sin(theta)),
                v1.z + (currRadius * glm::cos(phi)));
            ofBoxPrimitive b = box;
            b.set(currCubeSize, currCubeSize, currCubeSize);
            b.setPosition(
                v2.x + ((currCubeSize / 2) * glm::sin(phi) * glm::cos(theta)),
                v2.y + ((currCubeSize / 2) * glm::sin(phi) * glm::cos(theta)),
                v2.z + ((currCubeSize / 2) * glm::cos(phi)));
            b.lookAt(v1);
            b.draw();
        }
        currRadius *= 1.05;
        //currCubeSize *= 1.05;
    }
    cam.end();

    if (currFrame < numFrame) {
        currFrame++;

        if (isRecording) {
            img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }
    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("GIFs/Noise-Tube" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
    }
    ofPushStyle();
    ofSetColor(255, 0, 0);
    if (willRecord) ofDrawBitmapString("Record will start", 16, 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    /* 
    ofVec3f pos = cam.getGlobalPosition();
    ofVec3f lookAt = cam.getLookAtDir();
    std::cout << "cam.x: " << pos.x << " cam.y: " << pos.y << " cam.z: " << pos.z << endl;
    std::cout << "lookAt.x: " << lookAt.x << " lookAt.y: " << lookAt.y << " lookAt.z: " << lookAt.z << endl;
    */
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {  // s
        isExported = false;
        willRecord = true;
    } else if (key == 99) {
        ofxSaveCamera(cam, camPresetFile);
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}
//--------------------------------------------------------------
void ofApp::onGifSaved(string& fileName) {
    isSaved = true;
    cout << "Gif saved as " << fileName << endl;
    isSaved = false;
    isOptimizing = true;
    string path = ofFilePath::getCurrentExeDir() + "data/GIFs";
    string command = "gifsicle -O3 < " + path + fileName + " > " + path + "opt-" + fileName;
    cout << "GifSicle optimized version saved as opt-" << fileName << endl;
    ofSystem(command);
    isOptimizing = false;
    isExported = true;
}
//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}
//--------------------------------------------------------------