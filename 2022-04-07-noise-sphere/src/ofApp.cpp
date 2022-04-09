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
	ofSetFrameRate(25);

	cam.removeAllInteractions();
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
	cam.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
	cam.enableOrtho();
	cam.setNearClip(-1000000);
	cam.setFarClip(1000000);
	cam.setVFlip(true);


	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	screen.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	ofClear(0);
	screen.end();
	int total = 1000;

	for(int i = 0; i <= total; i++){
		float longitude = phi * i; // Set the longitude to the golden ration multiplied by each particle
		longitude -= floor(longitude); // Flooring it
		longitude *= TWO_PI; // Rotate 360 degrees ( Full Circle )
		float latitude = asin(-1.0 + 2.0 * i / total);

		Point newPoint;
		newPoint.pos = ofVec3f(
			radius * cos(latitude) * cos(longitude),
			radius * cos(latitude) * sin(longitude),
			radius * sin(latitude)
			);
		newPoint.col = palette[floor(palette.size() * ofRandom(1))];
		points.push_back(newPoint);

	}
}
//--------------------------------------------------------------
void ofApp::update(){
	ofEnableAlphaBlending();
	screen.begin();
	cam.begin();
	ofBackground(20, 26, 34);
	ofClear(20, 26, 34);
	frame(currFrame / static_cast <float>(numFrames));
	cam.end();
	screen.end();
	ofDisableAlphaBlending();

	if(currFrame < numFrames){
		currFrame++;

		if(isRecording){
			ofPixels pixels;
			screen.readToPixels(pixels);
			img.setFromPixels(pixels);
			gifEncoder.addFrame(img, 0.033f);
		}

	}else{
		currFrame = 0;

		if(isRecording){
			gifEncoder.save("noise-sphere" + ofGetTimestampString() + ".gif");
			isRecording = false;
		}
		if(willRecord){
			isRecording = true;
			willRecord = false;
		}
	}
}
// todo implement this https://therandomwalk.org/wp/3d-attractor-animation/
//--------------------------------------------------------------
void ofApp::frame(float t){
	for(Point p : points){

		ofSetColor(p.col);
		ofFill();
		ofDrawCircle(p.pos, 1);
	}
}
//--------------------------------------------------------------
void ofApp::draw(){
	screen.draw(0, 0);

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
	string command = "gifsicle -O3 < " + path + fileName + " > " + path + "/opt-" + fileName;
	ofSystem(command);
	cout << "GifSicle optimized version saved as opt-" << fileName << endl;
	isOptimizing = false;
	isExported = true;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 115){   // s
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