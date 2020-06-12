#include "ofApp.h"
//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetGlobalAmbientColor(ofColor(150, 150, 150));
    ofSetFrameRate(24);
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.50f, 156);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

    extRadius = ofGetWidth() / 2.0f;
    extRes = glm::half_pi<double>() / (numFrame + 1);
    //extRes = glm::pi<double>() / (numFrame + 1);
    outerSteps = floor(glm::half_pi<double>() / extRes);
    //outerSteps = floor(glm::pi<double>() / extRes);
    innerSteps = floor(glm::two_pi<double>() / res);
    noiseRadius = 20.0f;
    noiseScale = 400.0f;

    // Setup camera
    cam.setAutoDistance(false);
    bool isPresetLoaded = ofxLoadCamera(cam, camPresetFile);
    if (isPresetLoaded == false || 0) {
        std::cout << "Camera missing preset fallback" << endl;
        cam.setPosition(0, 0, ofGetHeight());
        cam.lookAt(ofVec3f(0, 0, 0));
    }
    arcs.clear();
    for (int i = 0; i <= outerSteps; i++) {
        // create arcs
        Arc a = Arc();
        a.setNoiseRadius(noiseRadius);
        a.setNoiseScale(noiseScale);
        arcs.push_front(a);

        // create particles
        int partsNum = (int)ofRandom(18);
        vector<Particle> arcParticles;
        for (int j = 0; j <= partsNum; j++) {
            float width = ofRandom(particleSize.x, particleSize.y);
            float height = ofRandom(particleSize.x, particleSize.y);

            Particle p = Particle(ofVec3f(0, -radius / 2, 0), width, height, radius);
            arcParticles.push_back(p);
        }
        particles.push_back(arcParticles);
    }
    light.setPointLight();
    light.setPosition(ofGetWidth() * 3, ofGetHeight() * 3, ofGetHeight() * 3);
    light.enable();
    ofSetLineWidth(2);
    ofNoFill();
}

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < particles.size(); i++) {
        for (int j = 0; j < particles[i].size(); j++) {
            particles[i][j].updateParticle();
        }
    }
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
        int arcId = (abs(i) + currFrame) % outerSteps;
        arcs[arcId].drawFromXandYRot(v1, xRot, yRot, currRadius, t);
        for (int j = 0; j < particles[arcId].size(); j++) {
            particles[arcId][j].drawFromXandYRot(v1, xRot, yRot, currRadius * 0.75, t, noiseScale, noiseRadius);
        }

        //currRadius += 12;
    }
    //ofDrawAxis(extRadius);
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
    noiseScale = x * 2;
    noiseRadius = (y / 24) + 1;
    for (Arc& arc : arcs) {
        arc.setNoiseScale(noiseScale);
        arc.setNoiseRadius(noiseRadius);
    }
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