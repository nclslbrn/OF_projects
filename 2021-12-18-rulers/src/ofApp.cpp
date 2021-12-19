#include "ofApp.h"


//--------------------------------------------------------------
float ofApp::ease(float p){
	return 3 * p * p - 2 * p * p * p;
}
//--------------------------------------------------------------
ofVec3f ofApp::getRandomSize(){
	return ofVec3f(ofRandom(size.x, size.y), ofRandom(size.x, size.y), ofRandom(size.x, size.y));
}
//--------------------------------------------------------------
ofVec3f ofApp::getRandomPosition(){
	return ofVec3f(ofRandomf() * domain, ofRandomf() * domain, ofRandomf() * domain);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(25);
	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();
	cam.setNearClip(0.01f);
	cam.setFarClip(100.0f);
	//cam.enableOrtho();
	cam.setGlobalPosition(ofVec3f(25.0f, 25.0f, -25.0f));
	cam.lookAt(ofVec3f(0, 0, 0));

	cubeDomain.set(domain * 2, domain * 2, domain * 2);

	for(int i = 0; i < numRuler; i++){
		before[i].position = getRandomPosition();
		before[i].size = getRandomSize();
		after[i].position = getRandomPosition();
		after[i].size = getRandomSize();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	if(ofGetFrameNum() > 0 && ((ofGetFrameNum() % numFrame) != 0)){

		t = (ofGetFrameNum() % numFrame) / static_cast <float>(numFrame);

		for(int i = 0; i < numRuler; i++){
			current[i].position = before[i].position.getInterpolated(after[i].position, t);
			current[i].size = before[i].size.getInterpolated(after[i].size, t);
		}

	}else{

		for(int i = 0; i < numRuler; i++){
			before[i].position = after[i].position;
			after[i].position = getRandomPosition();
			before[i].size = after[i].size;
			after[i].size = getRandomSize();
		}
		std::cout << "Loop end " << ofToString(ofGetFrameNum()) << endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	cam.begin();
	ofEnableDepthTest();
	ofSetColor(255, 75);
	// ofFill();
	// cubeDomain.drawWireframe();

	for(int i = 0; i < numRuler; i++){
		for(float z = 0; z < current[i].size.z; z += step){

			ofDrawRectangle(
				glm::vec3(
					current[i].position.x,
					current[i].position.y,
					current[i].position.z + z
					),
				current[i].size.x,
				current[i].size.y
				);
		}
		/*
		ofDrawBox(
			current[i].position,
			current[i].size.x,
			current[i].size.y,
			current[i].size.z
			);
		*/
	}
	ofDisableDepthTest();
	cam.end();

	ofDrawBitmapString(ofToString(t) + "/1", ofVec2f(12, 12));
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
