#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	img.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 1.0f, 8);
	// gifEncoder.setDitherMode(OFX_GIF_DITHER_BAYER4x4);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
}

//--------------------------------------------------------------
void ofApp::update(){

}
//--------------------------------------------------------------
void ofApp::draw(){
	float padding = 0.01;
	float t = (currFrame % animFrame) / static_cast <float>(animFrame);
	ofVec2f ctr(ofGetWidth() / 2, ofGetHeight() / 2);

	for(float x = -0.5f; x <= 0.5f; x += padding){
		for(float z = -0.5f; z <= 0.5f; z += padding){
			ofDrawBitmapString(
				"SHAPE",
				glm::vec3(
					(x * ctr.x) + ctr.x,
					0,
					(z * ctr.y) + ctr.y
					)
				);
		}
	}


	//ofDisableBlendMode();
	if(currFrame < animFrame){
		currFrame++;

		if(isRecording){
			img.grabScreen(0, 0, 800, 800);
			gifEncoder.addFrame(img, 0.066f);
		}
	}else{
		currFrame = 0;
		noiseScale = 0.01;

		if(isRecording){
			gifEncoder.save("VHS" + ofGetTimestampString() + ".gif");
			isRecording = false;
		}
		if(willRecord){
			isRecording = true;
			willRecord = false;
		}
	}

	ofPopMatrix();
	ofDrawBitmapString("Frame: " + ofToString(currFrame) + "/" + ofToString(animFrame), 16, ofGetHeight() - 48);
	ofPushStyle();
	ofSetColor(255, 0, 0);
	if(willRecord){
		ofDrawBitmapString("Record will start", 16, 16);
	}
	if(isRecording){
		ofDrawBitmapString("Recording", 16, 16);
	}
	if(isSaved){
		ofDrawBitmapString("Exported", 16, 16);
	}
	if(isOptimizing){
		ofDrawBitmapString("Optimizing", 16, 16);
	}
	if(isExported){
		ofDrawBitmapString("Exported", 16, 16);
	}
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::onGifSaved(string & fileName){
	isSaved = true;
	cout << "Gif saved as " << fileName << endl;
	isSaved = false;
	isOptimizing = true;
	string path = ofFilePath::getCurrentExeDir() + "data/";
	string command = "gifsicle -O3 < " + path + fileName + " > " + path + "opt-" + fileName + " --colors 256";
	cout << "GifSicle optimized version saved as opt-" << fileName << endl;
	ofSystem(command);
	isOptimizing = false;
	isExported = true;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 115){
		isExported = false;
		willRecord = true;
	}else{
		ofLogNotice() << "Unassigned key: " << key;
	}
}
//--------------------------------------------------------------
void ofApp::exit(){
	gifEncoder.exit();
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
