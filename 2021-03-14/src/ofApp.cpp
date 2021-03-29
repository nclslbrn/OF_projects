#include "ofApp.h"

#include "ofConstants.h"

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
    ofSetBackgroundColor(25);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    // initialize video player
    video.setLoopState(OF_LOOP_NORMAL);
    video.load("videos/pond-way-merce-cunningham-full-performance.mp4");
    video.setUseTexture(false);
    video.play();
    video.setPaused(true);
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
        for (int i = 0; i <= framesInLoop; i++) {
            if (video.isFrameNew()) {
                ofPixels framePixels = video.getPixels();
                FrameMesh j = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
                j.compute();
                frames.push_back(j);
                video.nextFrame();
                video.update();
            }
        }

        shader.load("shaders/particle");
        //ofDisableArbTex();
        ofEnableAlphaBlending();

        shader.begin();
        shader.setUniform2f("u_screenRes", ofGetWidth(), ofGetHeight());
        shader.setUniform1f("u_scale", meshScale);
        shader.end();

        camera.setDistance(ofGetWidth() * meshScale);
        camera.setFarClip(ofGetWidth() * meshScale);

        repulsor.resize(2);
        repulsor[0] = getRandomPos(center, meshScale);
        repulsor[1] = getRandomPos(center, meshScale);

        // setup our thread
        // nextFrame.setup();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    // Move frame forward
    video.nextFrame();
    video.update();
    // nextFrame.start();

    if (video.isFrameNew()) {
        for (int i = 0; i < framesInLoop; i++) {
            frames[i] = frames[i + 1];
        }
        ofPixels framePixels = video.getPixels();
        FrameMesh newFrame = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);

        newFrame.compute();
        frames[framesInLoop] = newFrame;
        // nextFrame.update(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
        // frames[framesInLoop] = nextFrame.frame;

        if (ofGetFrameNum() % numFrame == 0) {
            repulsor[0] = repulsor[1];
            repulsor[1] = getRandomPos(center, meshScale);
        }
    }
    // nextFrame.stop();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetBackgroundColor(0);
    // glCullFace(GL_CW);
    // glEnable(GL_CULL_FACE);

    float t = (ofGetFrameNum() % numFrame) / static_cast<float>(numFrame);
    ofVec2f repulsT = repulsor[0].getInterpolated(repulsor[1], t);

    camera.begin();
    shader.begin();

    shader.bindDefaults();

    for (int i = frames.size() - 1; i >= 0; i--) {
        ofPushMatrix();
        ofTranslate(center.x, center.y, i * 150);
        ofRotateX(180);

        if (frames[i].isTexAllocated()) {
            shader.setUniform1f("u_time", t);
            shader.setUniform1f("u_layer", i / (float)framesInLoop);
            shader.setUniform2f("u_frameRes", frames[i].getWidth(), frames[i].getHeight());
            shader.setUniform2f("u_mouse", (ofGetMouseX() - center.x) * meshScale, (ofGetMouseY() - center.y) * meshScale);
            shader.setUniform3f("u_camera", camera.getGlobalPosition());
            shader.setUniform2f("u_repulsor", repulsT);
            sparkTexture.getTexture().bind();
            shader.setUniformTexture("spark", sparkTexture.getTexture(), 2);
            shader.setUniformTexture("u_frameTex", frames[i].getTexture(), 0);
            frames[i].drawPoints();
        }
        ofPopMatrix();
    }

    sparkTexture.getTexture().unbind();
    shader.end();
    camera.end();

    ofDrawBitmapString(
        ofToString(ofGetFrameRate()) + " fps",
        20, 20);
    ofDrawBitmapString(
        ofToString(frames[0].getParticleNum()) + " particles",
        ofGetWidth() - 120, 20);
    ofDrawBitmapString(
        ofToString(t) + " / 1.0",
        20, ofGetHeight() - 20);
    ofDrawBitmapString(
        "[" + ofToString(repulsT.x) + ", " + ofToString(repulsT.y) + "]",
        ofGetWidth() - 200, ofGetHeight() - 20);
    // glDisable(GL_CULL_FACE);
}
//--------------------------------------------------------------
void ofApp::exit() {
    //thread.stopThread();
}