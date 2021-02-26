#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(20);
    ofDisableArbTex();
    mesh.setMode(OF_PRIMITIVE_POINTS);

    videoSample.loadMovie("video/blm@240p.mp4");
    videoSample.setSpeed(0.25);

    //videoSample.setLoopState(OF_LOOP_NORMAL);
    videoSample.play();
    //shader.allocate(ofGetWidth(), ofGetHeight());
    shader.load("shader/particle");
    nFrame = 150;
    res = 8;
    cam.setPosition(0, 0, 120);
    cam.setVFlip(true);
    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(3);
}

//--------------------------------------------------------------
void ofApp::update() {
    videoSample.update();

    if (videoSample.isFrameNew()) {
        frames.push_front(videoSample.getPixelsRef());
        if (frames.size() > nFrame) {
            frames.pop_back();
        }
    }
    if (!frames.empty()) {
        if (!imagePixels.isAllocated()) {
            imagePixels = frames[0];
        }
        int w = frames[0].getWidth();
        int h = frames[0].getHeight();
/*         voxels.clear();
 */
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                ofColor voxCol = frames[0].getColor(x, y);
                float brightness = voxCol.getBrightness();
               
                if( brightness > threshold ) {
                    numVisible++;

                }
                
                /* voxels.push_front(Voxel(voxPos, voxCol, size));
 */            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    //shader.setUniform1f("displaceRadius", displaceRadius);
    cam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2, -ofGetHeight() / 1.5);
   /*  for (int i = 0; i < voxels.size(); i++) {
        voxels[i].draw();
    } */
    ofDrawBitmapString(ofToString(numVisible), 16, 16);
    ofPopMatrix();
    cam.end();
    
    ofLog(OF_LOG_NOTICE, "numVisible is %d", numVisible); 
}
