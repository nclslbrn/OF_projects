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
    // initialize video player
    video.setLoopState(OF_LOOP_NORMAL);
    video.setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));
    video.loadMovie("videos/pond-way-merce-cunningham-full-performance.mp4");
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
        for (int i = 0; i < framesInLoop; i++) {
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
        ofDisableArbTex();
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
    video.nextFrame();
    video.update();
    if (video.isFrameNew()) {
        for (int i = framesInLoop; i > 0; i--) {
            frames[i] = frames[i - 1];
        }
        ofPixels framePixels = video.getPixels();
        FrameMesh newFrame = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
        newFrame.compute();
        frames[0] = newFrame;

        if (ofGetFrameNum() % numFrame == 0) {
            repulsor[0] = repulsor[1];
            repulsor[1] = getRandomPos(center, meshScale);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    float t = (ofGetFrameNum() % numFrame) / static_cast<float>(numFrame);
    ofVec2f repulsT = repulsor[0].getInterpolated(repulsor[1], t);

    if (isRecording) {
        frameCapture.begin();
        ofClear(0);
    }

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
            shader.setUniform2f("u_frameRes", frames[i].getWidth(), frames[i].getHeight());
            shader.setUniform2f("u_mouse", (ofGetMouseX() - center.x) * meshScale, (ofGetMouseY() - center.y) * meshScale);
            shader.setUniform3f("u_camera", camera.getGlobalPosition());
            shader.setUniform2f("u_repulsor", repulsT);
            sparkTexture.getTexture().bind();
            shader.setUniformTexture("spark", sparkTexture.getTexture(), 2);
            shader.setUniformTexture("u_frameTex", frames[i].getTexture(), 0);
            frames[i].drawFaces();
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
    ofDrawBitmapString(
        "[" + ofToString(repulsT.x) + ", " + ofToString(repulsT.y) + "]",
        ofGetWidth() - 200, ofGetHeight() - 20);
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