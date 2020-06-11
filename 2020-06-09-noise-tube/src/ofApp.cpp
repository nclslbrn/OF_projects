#include "ofApp.h"
//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    //ofSetGlobalAmbientColor(ofColor(150, 150, 150));
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 156);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

    extRadius = ofGetWidth() / 2.0f;
    extRes = glm::half_pi<double>() / numFrame;
    outerSteps = floor(glm::half_pi<double>() / extRes);
    innerSteps = floor(glm::two_pi<double>() / res);
    cubeSize = (glm::pi<float>() * 2 * radius) / innerSteps;
    noiseRadius = 20.0f;
    noiseScale = 400.0f;
    cam.setAutoDistance(false);

    bool isPresetLoaded = ofxLoadCamera(cam, camPresetFile);
    if (isPresetLoaded == false || 0) {
        std::cout << "Camera missing preset fallback" << endl;
        cam.setPosition(0, 0, ofGetHeight());
        cam.lookAt(ofVec3f(0, 0, 0));
    }
    arcs.clear();
    particles.clear();
    for (int i = 0; i <= outerSteps; i++) {
        Arc a = Arc();
        arcs.push_front(a);
        int partsNum = ofRandom(16);
        float theta0 = extRes * i;
        float xRot = glm::cos(theta0);
        float yRot = glm::sin(theta0);
        deque<Particle> stepParticles;

        for (int j = 0; j <= partsNum; j++) {
            ofVec3f v = ofVec3f(extRadius * xRot, ofGetHeight() + extRadius * yRot, 0);
            Particle p = Particle(v, ofRandomuf() * particleSize.x, ofRandomuf() * particleSize.y, radius);
            stepParticles.push_back(p);
        }
        particles.push_back(stepParticles);
    }
    light.setPointLight();
    light.setPosition(ofGetWidth() * 3, ofGetHeight() * 3, ofGetHeight() * 3);
    light.enable();
    ofNoFill();
    std::cout << "total ellipses: " << outerSteps << endl;
    std::cout << "tube diameter: " << glm::pi<float>() * 2 * radius << endl;
    std::cout << "cubeSize: " << cubeSize << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    float currRadius = radius;
    float t = currFrame % numFrame / static_cast<float>(numFrame);
    ofEnableDepthTest();
    ofBackground(ofColor::black);
    cam.begin();

    for (int i = 0; i >= -outerSteps; i--) {
        float theta0 = extRes * (i + t);
        float xRot = glm::cos(theta0);
        float yRot = glm::sin(theta0);

        ofVec3f v1 = ofVec3f(
            extRadius * xRot,
            ofGetHeight() + extRadius * yRot,
            0);
        int stuffIndex = (abs(i) + currFrame) % outerSteps;
        arcs[stuffIndex].drawFromXandYRot(v1, xRot, yRot, currRadius);
        /* deque<Particle> parts = particles[stuffIndex];
        for (int j = 0; j <= parts.size(); j++) {
            parts[j].draw();
        } */
        //currRadius *= 0.01;
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
    ofDrawBitmapString("Radius: " + ofToString(noiseRadius), 16, ofGetHeight() - 32);
    ofDrawBitmapString("Scale: " + ofToString(noiseScale), 16, ofGetHeight() - 16);

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
    noiseScale = x + 1;
    noiseRadius = (y / 12) + 1;
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    isDisplaced = false;
}
void ofApp::mouseReleased(int x, int y, int button) {
    isDisplaced = true;
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
    string path = ofFilePath::getCurrentExeDir() + "data/";
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