#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::nextConstant() {
    Constant constant = Constant(jsonFilename, constantsNum);
    if (constant.getA() && constant.getB() && constant.getC() && constant.getD()) {
        A = constant.getA();
        B = constant.getB();
        C = constant.getC();
        D = constant.getD();
        i = 0;
        std::cout << ofToString(A) << " " << ofToString(B) << " " << ofToString(C) << " " << ofToString(D) << endl;
        constantsNum++;
        imgName = "Constant-num-" + ofToString(constantsNum) + ".jpg";

        fbo.begin();
        ofClear(0);
        fbo.end();
    } else {
        ofLog(OF_LOG_ERROR, "There is a problem with the json file or every constants was used.");
        ofExit();
    }
}
//--------------------------------------------------------------
void ofApp::searchNewConstant() {
    A = ofRandom(4) - 2;
    B = ofRandom(4) - 2;
    C = ofRandom(4) - 2;
    D = ofRandom(4) - 2;
    i = 0;

    imgName = "A_" + ofToString(A) +
              "_B_" + ofToString(B) +
              "_C_" + ofToString(C) +
              "_D_" + ofToString(A) +
              ".jpg";

    std::cout << "A: " << A << " B:" << B << " C:" << C << " D:" << D << endl;

    fbo.begin();
    ofClear(0);
    fbo.end();
}
//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    iterations = 500000;
    width = 3840;
    height = 3840;
    steps = 500;

    // Set start position
    position.set(0, 0);
    // Set scale for polar coordinate
    minX = -4.0;
    minY = minX * height / width;
    maxX = 4.0;
    maxY = maxX * height / width;

    fbo.allocate(width, height, GL_RGB);

    ofSetColor(ofColor::white);
    ofFill();
    // Generate new constants for attracors
    if (findNewConstants) {
        searchNewConstant();
    } else {
        nextConstant();
    }
}
//--------------------------------------------------------------
void ofApp::update() {
    if (i < iterations) {
        fbo.begin();
        for (int j = 0; j < steps; j++) {
            float xn = glm::sin(A * position.y) + C * glm::cos(A * position.x);
            float yn = glm::sin(B * position.x) + D * glm::cos(B * position.y);

            float xi = (position.x - minX) * width / (maxX - minX);
            float yi = (position.y - minY) * height / (maxY - minY);

            // skip the first ten points
            if (i > 10) {
                ofDrawCircle(xi, yi, 2);
            }

            // save for the next iteration
            position.x = xn;
            position.y = yn;
        }
        i += steps;
        fbo.end();
    }

    if (i == iterations) {
        saveLargeImage(imgName);
        std::cout << "Image generated" << endl;
        if (findNewConstants) {
            searchNewConstant();
        } else {
            nextConstant();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    //ofBackground(0);
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofDrawBitmapString(ofToString(i) + "/" + ofToString(iterations), 16, ofGetHeight() - 16);
}

//--------------------------------------------------------------
void ofApp::saveLargeImage(string imgName) {
    unsigned char* pixels = new unsigned char[3840 * 3840 * 3];

    ofImage saveImage;
    saveImage.allocate(3840, 3840, OF_IMAGE_COLOR);
    saveImage.setUseTexture(false);

    fbo.begin();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);
    saveImage.setFromPixels(pixels, fbo.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR);
    saveImage.saveImage(imgName, OF_IMAGE_QUALITY_HIGH);
    fbo.end();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 32) {
        if (findNewConstants) {
            searchNewConstant();
        } else {
            nextConstant();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}
