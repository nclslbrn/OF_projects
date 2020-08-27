#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }

//--------------------------------------------------------------
void ofApp::cube(float d, float alpha) {
    float alphaScale = 255.0f;
    ofPushMatrix();
    ofTranslate(0, d / 2, d / 2);

    for (int i = 0; i < 4; i++) {
        ofColor sideColor = palette[i % 2];
        ofPushMatrix();
        ofSetColor(sideColor.r, sideColor.g, sideColor.b, (alphaScale * alpha));
        ofFill();
        ofRotateYRad(glm::half_pi<float>() * i);
        ofTranslate(-d / 2.0f, -d, d / 2.0f);
        ofDrawRectangle(0, 0, d, d);
        ofPopMatrix();
    }

    for (int i = 0; i < 2; i++) {
        ofColor topBottColor = palette[4];
        ofPushMatrix();
        ofSetColor(topBottColor.r, topBottColor.g, topBottColor.b, (alphaScale * alpha));
        ofFill();
        ofTranslate(0, -d / 2.0f, 0);
        ofRotateXRad(glm::half_pi<float>() + glm::pi<float>() * i);
        ofTranslate(-d / 2.0f, -d / 2.0f, d / 2.0f);
        ofDrawRectangle(0, 0, d, d);
        ofPopMatrix();
    }
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::setup() {
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

    // Animation setup
    //ofSetupScreenOrtho();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetGlobalAmbientColor(ofColor(255, 255, 255));
    ofDisableArbTex();

    cam.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, -ofGetHeight());
    cam.enableOrtho();
    cam.setNearClip(-1000000);
    cam.setFarClip(1000000);
    cam.setVFlip(true);
    cam.lookAt(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, ofGetHeight() / 2));

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);

    w = ofGetWidth() / N;
    h = ofGetHeight() / N;

    light.setPointLight();
    light.setDiffuseColor(ofColor(255, 255, 255));
    light.setPosition(0, 0, ofGetHeight());
    light.enable();
}

//--------------------------------------------------------------
void ofApp::update() {
    cache.begin();
    cam.begin();
    light.enable();
    ofClear(0, 127);
    ofBackground(0, 127);
    for (int n = 0; n < samplesPerFrame; n++) {
        float alpha = 1.0f - (float)n / (float)samplesPerFrame;
        float t = ofMap((currFrame + n * shutterAngle / (float)samplesPerFrame), 0, numFrames, 0, 1);
        animation(t, alpha, n);
    }
    light.disable();
    cam.end();
    cache.end();
    if (currFrame < numFrames) {
        currFrame++;

        if (isRecording) {
            ofPixels pixels;
            cache.readToPixels(pixels);
            img.setFromPixels(pixels);
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("cube-wall" + ofGetTimestampString() + ".gif");
            isRecording = false;
        }
        if (willRecord) {
            isRecording = true;
            willRecord = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cache.draw(0, 0);

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
void ofApp::animation(float t, float alpha, int n) {
    ofPushMatrix();
    ofTranslate(-w, ease(pow(t, 2)) * (-h * 2), 0);
    for (int x = 0; x <= N + 2; x++) {
        for (int y = 0; y <= N + 2; y++) {
            int index = (N / 2 * x) + y;
            float _t = (currFrame + index) % numFrames / static_cast<float>(numFrames);

            ofPushMatrix();
            ofTranslate((x * w) + (y % 2 == 0 ? w / 2.0f : 0), y * h, (N - y+1) * h);
            ofRotateXRad(glm::pi<float>() / 4);
            ofRotateYDeg(glm::pi<float>() / 4);
            ofRotateZRad((glm::pi<float>() / 4) + (glm::pi<float>() * ease(_t)));
            cube(w, alpha);
            ofPopMatrix();
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::onGifSaved(string& fileName) {
    isSaved = true;
    cout << "Gif saved as " << fileName << endl;
    isSaved = false;
    isOptimizing = true;
    string path = ofFilePath::getCurrentExeDir() + "data/";
    string command = "gifsicle -O3 < " + path + fileName + " > " + path + "/opt-" + fileName;
    ofSystem(command);
    cout << "GifSicle optimized version saved as opt-" << fileName << endl;
    isOptimizing = false;
    isExported = true;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115 || key == 83) {  // s || S
        isExported = false;
        willRecord = true;
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    gifEncoder.exit();
}
