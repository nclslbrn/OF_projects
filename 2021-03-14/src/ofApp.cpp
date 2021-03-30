#include "ofApp.h"

#include "ofConstants.h"
#include "ofGstVideoPlayer.h"
//--------------------------------------------------------------
ofVec2f ofApp::getRandomPos(ofVec2f c, float scale) {
    return ofVec2f(
        ((ofRandomuf() * ofGetWidth()) - c.x) * scale,
        ((ofRandomuf() * ofGetHeight()) - c.y) * scale);
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(false);
    ofSetFrameRate(25);
    ofDisableArbTex();
    ofSetBackgroundColor(25);
    ofEnablePointSprites();
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    // initialize video player
    video.setLoopState(OF_LOOP_NORMAL);
    video.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
    video.loadMovie("videos/pond-way-merce-cunningham-full-performance.mp4");
    video.setUseTexture(false);
    video.play();
    video.setPaused(true);

    shader.load("shaders/particle");

    if (video.isLoaded()) {
        frames.resize(framesInLoop);
        // load spark texture
        sparkTexture.allocate(248, 248, OF_IMAGE_COLOR_ALPHA);
        sparkTexture.load("textures/spark-2.png");
        sparkTexCoord = ofVec2f(
            sparkTexture.getWidth(),
            sparkTexture.getHeight());
        video.nextFrame();
        video.update();
        for (int i = 0; i < framesInLoop; i++) {
            if (video.isFrameNew()) {
                ofPixels framePixels = video.getPixels();
                FrameMesh j = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
                j.compute(shader);
                frames.push_back(j);
                video.nextFrame();
                video.update();
            }
        }

        ofDisableArbTex();
        ofEnableAlphaBlending();

        shader.begin();
        shader.setUniform2f("u_screenRes", ofGetWidth(), ofGetHeight());
        shader.setUniform1f("u_scale", meshScale);
        shader.end();

        camera.setDistance(ofGetWidth() * meshScale);
        camera.setFarClip(ofGetWidth() * meshScale);
        frameCapture.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
        toSave.allocate(ofGetWidth(), ofGetHeight(), ofImageType::OF_IMAGE_COLOR);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isRecording) {
        ofPixels pixels;
        frameCapture.readToPixels(pixels);
        toSave.setFromPixels(pixels);
        toSave.save("output/frameMesh-frame" + ofToString(ofGetFrameNum()) + ".jpg", OF_IMAGE_QUALITY_BEST);
    }
    video.update();
    if (video.isFrameNew()) {
        for (int i = framesInLoop; i > 0; i--) {
            frames[i] = frames[i - 1];
        }
        ofPixels framePixels = video.getPixels();
        FrameMesh newFrame = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
        newFrame.compute(shader);
        frames[0] = newFrame;
    }
    video.nextFrame();
}

//--------------------------------------------------------------
void ofApp::draw() {
    float t = (ofGetFrameNum() % numFrame) / static_cast<float>(numFrame);
    if (isRecording) {
        frameCapture.begin();
    }

    ofClear(0);
    camera.begin();
    shader.begin();
    shader.bindDefaults();

    for (int i = 0; i < frames.size(); i++) {
        ofPushMatrix();
        ofTranslate(center.x, center.y, i * 150);
        ofRotateX(180);

        if (frames[i].isTexAllocated()) {
            shader.setUniform1f("u_time", t);
            shader.setUniform1f("u_layer", i / (float)framesInLoop);
            sparkTexture.getTexture().bind();
            shader.setUniformTexture("spark", sparkTexture.getTexture(), 2);
            shader.setUniformTexture("u_frameTex", frames[i].getTexture(), 0);
            //frames[i].drawFaces();
            frames[i].drawPoints();
        }
        ofPopMatrix();
    }

    sparkTexture.getTexture().unbind();
    shader.end();
    camera.end();
    if (isRecording) {
        frameCapture.end();
        frameCapture.draw(0, 0);
    }
    ofDrawBitmapString(
        ofToString(ofGetFrameRate()) + " fps",
        20, 20);
    ofDrawBitmapString(
        ofToString(frames[0].getParticleNum()) + " particles",
        ofGetWidth() - 120, 20);
    ofDrawBitmapString(
        ofToString(t) + " / 1.0",
        20, ofGetHeight() - 20);
    if (isRecording) {
        ofDrawBitmapString("Recording", ofGetWidth() - 120, ofGetHeight() - 20);
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 114) {
        isRecording = !isRecording;
        std::cout << (isRecording ? "start recording" : "stop recording") << endl;
    } else {
        std::cout << "Unaffected key = " << key << endl;
    }
}

//--------------------------------------------------------------

void ofApp::exit() {}