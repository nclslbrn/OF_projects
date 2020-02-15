#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(25);


    animFrame = 120;
    radius = 0.5;
    noiseScale = 0.005;
    step = 1;
    width = ofGetWidth();
    height = ofGetHeight();
    img.allocate(width, height, OF_IMAGE_GRAYSCALE);

    isRecoding = false;

}

//--------------------------------------------------------------
void ofApp::update(){

    int frameDrawn = (int) ofGetFrameNum() < animFrame ? (int) ofGetFrameNum() : (int) ofGetFrameNum() % animFrame;
    float t = ofMap(frameDrawn, 0, animFrame, 0, 1);
   
    img.setColor(ofColor::black);

    for( int x = 0; x < width; x += step ){
        for( int y = 0; y < height; y += step ) {

            float noiseValue = ofNoise(     
                x * noiseScale,
                y * noiseScale,
                radius * (glm::cos(t * (glm::pi<float>()*2))),
                radius * (glm::sin(t * (glm::pi<float>()*2)))
            );
            int roundNoise = roundf(noiseValue * 100);
            bool b = (roundNoise % 5 ) == 0;

            if( b ) {
                for( int _x = 0; _x < step; _x++ ){
                    for( int _y = 0; _y < step; _y++ ){
                        int sx = x + _x;
                        int sy = y + _y;
                        img.setColor(sx, sy, ofColor::white);
                    }
                }
            }
        }
    }
    img.update();

    if( ofGetFrameNum() <= animFrame && isRecoding) {
        img.saveImage("frame-"+ ofToString(ofGetFrameNum()) + ".png");
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    img.draw(0, 0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    radius = ofMap(mouseX, 0, ofGetWidth(), 0.01, 0.5);
    noiseScale = ofMap(mouseY, 0, ofGetHeight(), 0.001, 0.005);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}