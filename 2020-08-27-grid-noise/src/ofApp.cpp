#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(25);
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);

    cam.setPosition(0, 0, -ofGetHeight() * 0.75);
    cam.setVFlip(false);
    cam.lookAt(ofVec3f(0, ofGetHeight() * 0.25, ofGetHeight()));
    //cam.setFarClip(ofGetHeight());

    cellSize = (ofGetHeight() * 8 / rows);
    hSize = cellSize;
    for (int l = 0; l < numLayer; l++) {
        layers.push_back(
            Layer(cols, rows, cellSize, noiseDepth, l));
        noiseDepth += depthStep;
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    float t = (ofGetFrameNum() % numFrame) / static_cast<float>(numFrame);
    int layerId = 0;
    vector<int> layerToRemove;

    for (auto& layer : layers) {
        if (layer.getZpos() > numLayer) {
            layer.update(noiseDepth);
            layerToRemove.push_back(layerId);
            noiseDepth += depthStep;
        } else {
            layer.update();
            //layer.update(noiseDepth);
        }

        layerId++;
    }
    for (int l = 0; l < layerToRemove.size(); l++) {
        for (int i = 0; i < layers.size(); i++) {
            if (layerToRemove[l] == i) {
                layers[i] = Layer(cols, rows, cellSize, noiseDepth, l);
            }
        }
    }

    cache.begin();
    cam.begin();
    ofEnableDepthTest();
    ofClear(50);
    ofBackground(50);

    ofPushMatrix();
    ofRotateX(45.0f);
    ofTranslate(cols * cellSize * -0.5, 0, t * hSize);

    for (int i = 0; i < layers.size(); i++) {
        ofPushStyle();
        ofSetColor(255 / numLayer * i, 100);
        ofSetLineWidth(i / 4.0f);
        std::cout << "Layer n°" << i << " Z => " << layers[i].getZpos() << endl;
        vector<ofVec2f> points = layers[i].getPoints(noiseThreshold, -1, -1);

        for (ofVec2f point : points) {
            vector<ofVec2f> nearPoints = layers[i].getPoints(noiseThreshold, point.x, point.y);
            //vector<ofVec2f> bottomPoints = layers[i + 1].getPoints(noiseThreshold, point.x, point.y);
            ofDrawLine(
                point.x * cellSize,
                point.y * cellSize,
                (numLayer - i) * hSize,
                point.x * cellSize,
                point.y * cellSize,
                (numLayer - i + 1) * hSize);
            for (ofVec2f nPoint : nearPoints) {
                ofDrawLine(
                    point.x * cellSize,
                    point.y * cellSize,
                    (numLayer - i) * hSize,
                    nPoint.x * cellSize,
                    nPoint.y * cellSize,
                    (numLayer - i) * hSize);
            }
            /*
            for (ofVec2f bPoint : bottomPoints) {
                    ofDrawLine(
                        point.x * cellSize, point.y * cellSize, 0,
                        bPoint.x * cellSize, bPoint.y * cellSize, cellSize);
                } */
        }
        ofPopStyle();
    }
    ofPopMatrix();
    cam.end();
    cache.end();
    if (currFrame < numFrame) {
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
    cache.draw(0, 0, ofGetWidth(), ofGetHeight());

    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString(ofGetFrameRate(), 16, ofGetHeight() - 16);
    if (willRecord) ofDrawBitmapString("Record will start", 16, 16);
    if (isRecording) ofDrawBitmapString("Recording", 16, 16);
    if (isSaved) ofDrawBitmapString("Exported", 16, 16);
    if (isOptimizing) ofDrawBitmapString("Optimizing", 16, 16);
    if (isExported) ofDrawBitmapString("Exported", 16, 16);
    ofPopStyle();
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