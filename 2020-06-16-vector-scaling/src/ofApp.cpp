#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p) { return 3 * p * p - 2 * p * p * p; }
//--------------------------------------------------------------
ofVec2f ofApp::circle(float n) {
    return ofVec2f(glm::cos(n), glm::sin(n));
}
//--------------------------------------------------------------
ofVec2f ofApp::astroid(float n) {
    return ofVec2f(
        pow(glm::sin(n), 3),
        pow(glm::cos(n), 3));
}
//--------------------------------------------------------------
ofVec2f ofApp::quadrifolium(float n) {
    return ofVec2f(
        glm::sin(n * 2) * glm::cos(n),
        glm::cos(n) * glm::sin(n * 2));
}
//--------------------------------------------------------------
ofVec2f ofApp::rect_hyperbola(float n) {
    float sec = 1 / glm::sin(n);

    float xt = 1 / glm::sin(n);
    float yt = glm::tan(n);

    return ofVec2f(xt, yt);
}
//--------------------------------------------------------------
ofVec2f ofApp::trifolium(float n) {
    return ofVec2f(
        -1.0f * glm::cos(n) * glm::cos(3.0 * n),
        glm::sin(n) - glm::cos(3.0f * n));
}
//--------------------------------------------------------------

void ofApp::setup() {
    //  debug slowly
    // ofSetFrameRate(12);
    gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    cache.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    font.load("fonts/OperatorMono-Medium.otf", 18);
    for (float x = -3; x <= 3; x += density) {
        for (float y = -3; y <= 3; y += density) {
            pos.push_back(ofVec2f(
                x + ofRandom(1) * 0.03,
                y + ofRandom(1) * 0.03));
            colors.push_back(floor(palette.size() * ofRandom(1)));
        }
    }
    cache.begin();
    ofClear(0);
    cache.end();
}

//--------------------------------------------------------------
void ofApp::update() {
    ofEnableAlphaBlending();
    cache.begin();
    ofBackground(0);
    ofClear(0);
    for (int n = 0; n < samplesPerFrame; n++) {
        float alpha = 1.0 - (float)n / (float)samplesPerFrame;
        float t = ofMap((currFrame + n * shutterAngle / (float)samplesPerFrame), 0, numFrames, 0, 1);
        //float t = ((currFrame + n) % numFrames) / static_cast<float>(numFrames);
        animation(t, alpha);
    }
    cache.end();
    ofDisableAlphaBlending();

    if (currFrame < numFrames) {
        currFrame++;

        if (isRecording) {
            ofPixels pixels;
            cache.readToPixels(pixels);
            img.setFromPixels(pixels);
            //img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            gifEncoder.addFrame(img, 0.033f);
        }

    } else {
        currFrame = 0;

        if (isRecording) {
            gifEncoder.save("vector-scaling" + ofGetTimestampString() + ".gif");
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
void ofApp::animation(float t, float alpha) {
    /* Reinit pos */
    /* if (t == 0) {
        int p = 0;
        for (float x = -3.0f; x <= 3.0f; x += density) {
            for (float y = -3.0f; y <= 3.0f; y += density) {
                pos[p] = ofVec2f(x + (ofRandom(1) * 0.03f), y + (ofRandom(1) * 0.03f));
                p++;
            }
        }
    } 
    float te = ease(t + t);
    float et = ease(2.0 - (t + t));    
    */
    int index = 0;
    for (ofVec2f& p : pos) {
        float xx = ofMap(p.x, -6.5, 6.5, (width / 2.0) - width, (width / 2.0) + width);
        float yy = ofMap(p.y, -6.5, 6.5, (height / 2.0) - height, (height / 2.0) + height);

        ofSetColor(
            palette[colors[index]].r,
            palette[colors[index]].g,
            palette[colors[index]].b,
            180 * alpha);
        ofFill();
        ofDrawCircle(xx, yy, 1);

        float scale = 0.01;
        float rx = 15 * glm::cos(glm::pi<float>() * t);
        float ry = 15 * glm::sin(glm::pi<float>() * t);

        // third noise arg: t * glm::two_pi<float>()
        float n1a = 15 * ofMap(ofNoise(p.x / 30, p.y / 30, rx, ry), 0, 1, -1, 1);
        float n1b = 15 * ofMap(ofNoise(p.y / 30, p.x / 30, rx, ry), 0, 1, -1, 1);

        ofVec2f v1 = rect_hyperbola(n1a);
        ofVec2f v2 = astroid(n1b);
        ofVec3f v3 = quadrifolium(n1a);
        ofVec3f v4 = trifolium(n1b);
        //  float n2a = 12 * ofMap(ofNoise(v1.x, v1.y, rx, ry), 0, 1, -1, 1);
        //  float n2b = 12 * ofMap(ofNoise(v2.x, v2.y, rx, ry), 0, 1, -1, 1);

        ofVec2f diff1 = v2 - v1;
        ofVec2f diff2 = v3 - v4;
        diff1 *= 0.3;
        diff2 *= 0.3;

        ofVec2f v = diff1.interpolate(diff2, t);
        //  float n3 = 3 * ofMap(ofNoise(v3.x, v3.y, t), 0, 1, -1, 1);  // , t * glm::two_pi<float>()
        //  float n3a = 3 * ofMap(ofNoise(v2.x, v2.y, t * glm::two_pi<float>()), 0, 1, -1, 1);
        //  ofVec2f v = ofVec2f(n2a, n2b);
        //  ofVec2f v = circle(n2b);
        //  ofVec2f v = ofVec2f(cos(n2), sin(n3b));
        //ofVec2f v = circle(n3);
        p.x += scale * v.x;
        p.y += scale * v.y;
        index++;
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {  // s
        isExported = false;
        willRecord = true;
    } else {
        ofLogNotice() << "Unassigned key: " << key;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    //split = ofVec2f(x / (float)ofGetWidth(), y / (float)ofGetHeight());
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
void ofApp::exit() {
    gifEncoder.exit();
}
