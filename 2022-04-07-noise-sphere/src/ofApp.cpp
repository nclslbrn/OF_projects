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
ofVec2f ofApp::init(){
	float theta = ofRandomuf() * glm::two_pi <float>();
	float r = ofRandomuf() / sample;
	return ofVec2f(cos(theta) * r, sin(theta) * r);
}
//--------------------------------------------------------------
ofVec2f ofApp::move(ofVec2f pos, float t){
	float tt = glm::two_pi <float>() * t;
	float a0 = atan2(pos.y, pos.x);

	float a1 = ofMap(
		ofNoise(
			pos.x * frequency,
			pos.y * frequency,
			cos(tt) / sample,
			sin(tt) / sample
			),
		0, 1, -1, 1);
	ofVec2f v1 = astroid(pos / sample);
	ofVec2f v2(cos(a1 * turbulence), sin(a1 * turbulence));
	ofVec2f v3 = v1.getInterpolated(v2, 0.5);
	ofVec2f v4(sin(v3.x) * sample, sin(v3.y) * sample);
	pos.x += v4.x * 0.007;
	pos.y += v4.y * 0.007;

	return pos;
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(25);

	gifEncoder.setup(ofGetWidth(), ofGetHeight(), 0.25f, 8);  // colors = 8
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	screen.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	ofClear(0);
	screen.end();

	for(int i = 0; i < NUM_POINTS; i++){
		Point newPoint;
		newPoint.pos = init();
		newPoint.col = palette[floor(palette.size() * ofRandom(1))];
		points[i] = newPoint;
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	ofEnableAlphaBlending();
	screen.begin();
	ofBackground(20, 26, 34);
	ofClear(20, 26, 34);
	frame(currFrame / static_cast <float>(numFrames));
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
//--------------------------------------------------------------
void ofApp::frame(float t){
	for(int i = 0; i < NUM_POINTS; i++){
		ofVec2f pos = points[i].pos;
		for(int j = 0; j < trails; j++){

			float xx = ofMap(pos.x, -sample, sample, margin, ofGetWidth() - margin);
			float yy = ofMap(pos.y, -sample, sample, margin, ofGetHeight() - margin);

			float dist = sqrt(
				pow(abs(ofGetWidth() * 0.5f - xx), 2)
				+ pow(abs(ofGetHeight() * 0.5f - yy), 2)
				);
			if(dist > radius){
				pos = init();
			}
			ofSetColor(points[i].col);
			ofFill();
			ofDrawCircle(xx, yy, 1);

			pos = move(pos, t);
		}
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
//--------------------------------------------------------------

ofVec2f ofApp::circle(float n){
	return ofVec2f(glm::cos(n), glm::sin(n));
}
//--------------------------------------------------------------
ofVec2f ofApp::astroid(ofVec2f v){
	return ofVec2f(
		pow(glm::sin(v.x), 3),
		pow(glm::cos(v.y), 3));
}
//--------------------------------------------------------------
ofVec2f ofApp::quadrifolium(ofVec2f v){
	return ofVec2f(
		glm::sin(v.x * 2) * glm::cos(v.x),
		glm::cos(v.y) * glm::sin(v.y * 2));
}
//--------------------------------------------------------------
ofVec2f ofApp::rect_hyperbola(ofVec2f v){
	float theta = atan2(v.x, v.y);
	float sec = 1 / glm::cos(theta);
	return ofVec2f(sec, glm::tan(theta));
}
//--------------------------------------------------------------
ofVec2f ofApp::trifolium(ofVec2f v){
	return ofVec2f(
		-1.0f * glm::cos(v.x) * glm::cos(3.0 * v.x),
		glm::sin(v.y) - glm::cos(3.0f * v.y));
}
//--------------------------------------------------------------
ofVec2f ofApp::cardioid(ofVec2f v){
	return ofVec2f(
		1.0f - glm::cos(v.x) * glm::cos(v.x),
		glm::sin(v.y) * (1.0f - glm::cos(v.y)));
}
//--------------------------------------------------------------
ofVec2f ofApp::deltoid(ofVec2f v){
	return ofVec2f(
		(2 * cos(v.x) / 3.0f) + (1 / 3.0f * cos(2 * v.x)),
		(2 * sin(v.y) / 3.0f) - (1 / 3.0f * sin(2 * v.y)));
}
//--------------------------------------------------------------
ofVec2f ofApp::ranunculoid(ofVec2f v){
	return ofVec2f(
		6.0f * glm::cos(v.x) - glm::cos(6.0f * v.y),
		6.0f * glm::sin(v.y) - glm::sin(6.0f * v.y));
}
//--------------------------------------------------------------
ofVec2f ofApp::cycloid(ofVec2f v){
	return ofVec2f(
		v.x - glm::sin(v.x),
		1.0f - glm::cos(v.y));
}
//--------------------------------------------------------------