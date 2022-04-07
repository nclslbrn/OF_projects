#include "ofApp.h"

//--------------------------------------------------------------
float ofApp::ease(float p){
	return 3 * p * p - 2 * p * p * p;
}
//--------------------------------------------------------------
float ofApp::softPlus(float q, float p){
	float qq = q + p;
	if(qq <= 0){
		return 0;
	}
	if(qq >= 2 * p){
		return qq - p;
	}
	return 1 / (4 * p) * qq * qq;
}
//--------------------------------------------------------------
void ofApp::setup(){
	for(int i = 0; i < NUM_POINTS; i++){
		float d = ofRandomuf() * radius;
		float theta = ofRandomuf() * glm::two_pi <float>();
		float gama = ofRandomuf() * glm::two_pi <float>();
		ofVec3f pos(
			ofGetWidth() / 2 + d * glm::cos(theta) * glm::cos(gama),
			ofGetHeight() / 2 + d * glm::cos(theta) * glm::sin(gama),
			ofGetHeight() / 2 + d * glm::sin(theta)
			);
		ofColor col = palette[floor(palette.size() * ofRandom(1))];
		Point newPoint;
		newPoint.pos = pos;
		newPoint.col = col;
		points[i] = newPoint;
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	int frameDrawn = (int)ofGetFrameNum() < animFrame
		? (int)ofGetFrameNum()
		: (int)ofGetFrameNum() % animFrame;

	float t = ofMap(frameDrawn, 0, animFrame, 0, 1);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	ofEnableAlphaBlending();
	for(int i = 0; i < NUM_POINTS; i++){
		ofSetColor(points[i].col);
		ofFill();
		ofDrawCircle(points[i].pos, 1);
	}
	ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
