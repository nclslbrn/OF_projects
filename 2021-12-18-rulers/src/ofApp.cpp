#include "ofApp.h"


//--------------------------------------------------------------
float ofApp::ease(float p, int g = 0){
	if(g > 0){
		return p < 0.5
			? 0.5 * pow(2 * p, g)
			: 1 - 0.5 * pow(2 * (1 - p), g);
	}
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

	cubeDomain.set(domain * 2.0f, domain * 2.0f, domain * 2.0f);

	for(int i = 0; i < numRuler; i++){
		before[i].position = getRandomPosition();
		before[i].size = getRandomSize();
		after[i].position = getRandomPosition();
		after[i].size = getRandomSize();
		colors[i] = ofColor(ofRandom(0, 255), 25);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	if(ofGetFrameNum() > 0 && ((ofGetFrameNum() % numFrame) != 0)){

		float t = 1.0f * (ofGetFrameNum() % numFrame) / static_cast <float>(numFrame);

		for(int i = 0; i < numRuler; i++){
			current[i].position = before[i].position.getInterpolated(after[i].position, ease(t, 5));
			current[i].size = before[i].size.getInterpolated(after[i].size, ease(t, 5));
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
	// ofFill();
	// cubeDomain.drawWireframe();

	for(int i = 0; i < numRuler; i++){
		ofSetColor(colors[i]);

		/* for(float x = 0; x < current[i].size.x; x += step){
			for(float y = 0; y < current[i].size.y; y += step){
				for(float z = 0; z < current[i].size.z; z += step){
					ofDrawCircle(
						current[i].position.x + x,
						current[i].position.y + y,
						current[i].position.z + z,
						step * 0.5
						);
				}
			}
		} */


		ofDrawBox(
			current[i].position,
			current[i].size.x,
			current[i].size.y,
			current[i].size.z
			);

	}
	ofDisableDepthTest();
	cam.end();
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
