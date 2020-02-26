#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::reset() {
    A = ofRandom(4) - 2;
    B = ofRandom(4) - 2;
    C = ofRandom(4) - 2;
    D = ofRandom(4) - 2;
    iterations = 50000;
    step = 100;
    i = 0;

    imgName = "A_"+ofToString(A)+"_B_"+ofToString(B)+"_C_"+ ofToString(C)+"_D_"+ofToString(A)+".png";

    img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_GRAYSCALE);
    img.setColor(ofColor::black);
    img.update();
    
    std::cout << "A: " << ofToString(A) << " B:" << ofToString(B) << " C:" << ofToString(C) << " D:" << ofToString(D) << endl;
}
//--------------------------------------------------------------
void ofApp::increasePixelBrightness(int x, int y, int amount) {
    ofColor pxColor = img.getColor(x, y);
    int pxBrightness = pxColor.getBrightness();
    pxColor.setBrightness( pxBrightness + amount);
    img.setColor(x, y, pxColor);
}
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    
    // Generate new constants for attracors
    reset();
    // Set start position
    position.set(0, 0);
    // Set scale for polar coordinate
    minX = -4.0;
    minY = minX * ofGetHeight() / ofGetWidth();
    maxX = 4.0;
    maxY = maxX * ofGetHeight() / ofGetWidth();

    // GUI
    reinitConstantsButton.addListener(this, &ofApp::reinitConstantsButtonClick);
    saveImageButton.addListener(this, &ofApp::saveImageButtonClick);
    saveConstantsButton.addListener(this, &ofApp::saveConstantsButtonClick);
    gui.setup("Controls");
    gui.add(reinitConstantsButton.setup("Reset constant"));
    gui.add(saveImageButton.setup("Save image"));
    gui.add(saveConstantsButton.setup("Save constants"));

    //std::string dbFilePath = "constants.json";
    //bool parsingSuccess = dbFile.open(dbFilePath);

}
//--------------------------------------------------------------
void ofApp::exit() {
    reinitConstantsButton.removeListener(this, &ofApp::reinitConstantsButtonClick);
    saveImageButton.removeListener(this, &ofApp::saveImageButtonClick);
    saveConstantsButton.removeListener(this, &ofApp::saveConstantsButtonClick);
}
//--------------------------------------------------------------
void ofApp::update() {
    if( i < iterations ) {
        for( int j = 0; j < step; j++ ) {
            float xn = glm::sin(A * position.y) + C * glm::cos(A * position.x);
            float yn = glm::sin(B * position.x) + D * glm::cos(B * position.y);

            float xi = (position.x - minX) * ofGetWidth() / (maxX - minX);
            float yi = (position.y - minY) * ofGetHeight() / (maxY - minY);

            // skip the first ten points
            if (i > 10) {
                increasePixelBrightness(xi-1,   yi,     50);
                increasePixelBrightness(xi,     yi-1,   50);
                increasePixelBrightness(xi+1,   yi,     50);
                increasePixelBrightness(xi,     yi+1,   50);

                increasePixelBrightness(xi,     yi,     170);

            }
            // save for the next iteration
            position.x = xn;
            position.y = yn;
        }
        i+=step;
        img.update();
    } 

    if (i == iterations) {
        img.save(imgName);
        std::cout << "Image generated" << endl;
        reset();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    img.draw(0, 0);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::reinitConstantsButtonClick() {
    reset();
}
//--------------------------------------------------------------
void ofApp::saveImageButtonClick() {
    img.save(imgName);
    std::cout << "Image generation : " << ofToString(i) << "/" << ofToString(iterations) << endl;
}
//--------------------------------------------------------------
void ofApp::saveConstantsButtonClick() {
    //if( parsingSuccess ) {
        //TODO: create constants class with method to get ad save A, B, C & D
        //dbFile.push_front()
    //}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
