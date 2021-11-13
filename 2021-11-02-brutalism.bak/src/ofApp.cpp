#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	screenWidth = 2880;
	screenHeight = 1620;
	loop = 0;
	numLoop = 20;
	isRecording = false;
	showInfo = false;
	ofSetWindowShape(screenWidth, screenHeight);
	center = ofVec2f(screenWidth / 2, screenHeight / 2);

	string imageFile = imageSource[(int)ofRandom(0, sizeof(imageSource) / sizeof(imageSource[0]))];
	// std::cout << imageFile << endl;
	sample.load("images/2880x1620/" + imageFile);

	nShortAudioSample = shortAudioDir.listDir("audio-sample/short");
	//nLongAudioSample = longAudioDir.listDir("audio-sample/long");
	//std::cout << "Short audio " + ofToString(nShortAudioSample) << endl;
	//std::cout << "Long audio " + ofToString(nLongAudioSample) << endl;
	shortAudioSample = new ofSoundPlayer[nShortAudioSample];
	//longAudioSample = new ofSoundPlayer[nLongAudioSample];
	for(int i = 0; i < nShortAudioSample; i++){
		string shortSoundFile = shortAudioDir.getPath(i);
		// std::cout << shortAudioDir.getPath(si) << endl;
		shortAudioSample[i].setMultiPlay(true);
		//shortAudioSample[i].setLoop(true);
		shortAudioSample[i].loadSound(shortSoundFile);
		shortAudioSample[i].stop();
	}
	/* for(int i = 0; i < nLongAudioSample; i++){
		string longSoundFile = shortAudioDir.getPath(i);
		// std::cout << longAudioDir.getPath(si) << endl;
		longAudioSample[i].setMultiPlay(false);
		longAudioSample[i].setLoop(true);
		longAudioSample[i].loadSound(
			longSoundFile,
			true
			);
	} */


	screenShader.load("shaders/Screen");
	billboardShader.load("shaders/Billboard");

	screen.set(screenWidth, screenHeight);
	screen.setPosition(center.x, center.y, 0);
	screen.setScale(1, -1, 1);
	screen.mapTexCoords(0, 0, sample.getWidth(), sample.getHeight());

	// of/exmaples/billboardExemple
	billboards.getVertices().resize(NUM_BILLBOARDS);
	billboards.getColors().resize(NUM_BILLBOARDS);
	billboards.getNormals().resize(NUM_BILLBOARDS, glm::vec3(0));
	billboards.setUsage(GL_DYNAMIC_DRAW);
	billboards.setMode(OF_PRIMITIVE_POINTS);

	// of/addons/ofxTextureRecorder/example
	capture.allocate(screenWidth, screenHeight, GL_RGB);
	ofxTextureRecorder::Settings settings(capture.getTexture());
	settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
	settings.numThreads = 2;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);
	nextMove();
}
//--------------------------------------------------------------
void ofApp::nextMove(){
	// Graphics
	isVertical = ofRandomuf() > 0.5;
	size = (int)(1 + (ofRandomuf() * (isVertical ? screenHeight : screenWidth) * 0.015));
	stepSize = (int)(1 + ofRandomuf() * 320);
	numFrame = 6 * ceil(ofRandomuf() * 16);
	goForward = ofRandomuf() > 0.5;
	d = 0;
	float sampleWidth = isVertical ? size : stepSize;
	float sampleHeight = isVertical ? stepSize : size;
	rect.setX(ofRandom(0, screenWidth - sampleWidth));
	rect.setY(ofRandom(0, screenHeight - sampleHeight));
	rect.setWidth(sampleWidth);
	rect.setHeight(sampleHeight);

	sampleCroped = sample;
	sampleCroped.crop(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
	sample.getPixels().cropTo(crop, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

	for(int i = 0; i < NUM_BILLBOARDS; i++){
		size_t x = rect.getX() + (ofRandom(0, sampleWidth));
		size_t y = rect.getY() + (ofRandom(0, sampleHeight));

		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			sample.getColor(x, y));
		billboardSizeTarget[i] = stepSize * ofRandomuf();
	}
	// Sounds
	if(shortAudioSample[currentShortSample].isPlaying()){
		shortAudioSample[currentShortSample].stop();
	}

	currentShortSample = (int)ofRandom(0, nShortAudioSample - 1);
	//currentLongSample = (int)ofRandom(0, nLongAudioSample);
	if(ofGetFrameNum() > 0 && shortAudioSample[currentShortSample].isLoaded()){
		shortAudioSample[currentShortSample].play();
	}

	//}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	if((int)ofGetFrameNum() % numFrame == 0){
		nextMove();
		loop++;

	}else{
		ofVec2f displace(
			rect.getX() + (goForward ? 1 : -1) * (isVertical ? d : 0) * rect.getWidth(),
			rect.getY() + (goForward ? 1 : -1) * (isVertical ? 0 : d) * rect.getHeight()
			);

		if(displace.x < 0){
			displace.x += sample.getWidth();
		}
		if(displace.y < 0){
			displace.y += sample.getHeight();
		}
		if(displace.x > screenWidth){
			displace.x = 1.0f * (static_cast <int>(round(displace.x)) % screenWidth);
		}
		if(displace.y < screenHeight){
			displace.y = 1.0f * (static_cast <int>(round(displace.y)) % screenHeight);
		}

		crop.pasteInto(sample.getPixels(), displace.x, displace.y);
		sample.update();
		d++;

		float t =
			1.0f - ((ofGetFrameNum() % numFrame) / static_cast <float>(numFrame));

		for(int i = 0; i < NUM_BILLBOARDS; i++){

			glm::vec3 vec(0, t + ofSignedNoise(billboards.getVertex(i).x, billboards.getVertex(i).y, 0, 0), 0);
			billboardVels[i] += vec;
			// billboardVels[i].y *= 1.01f;
			billboards.getVertices()[i] += billboardVels[i];
			billboardVels[i] *= 0.94f;
			billboards.setNormal(
				i, glm::vec3(12 + billboardSizeTarget[i] * ofNoise(t + i), 0, 0));
		}

		capture.begin();
		ofClear(0, 255);
		nextFrame();
		capture.end();

		if(isRecording){
			if(ofGetFrameNum() > 0){
				recorder.save(capture.getTexture());
			}
		}
	}

}
//--------------------------------------------------------------

void ofApp::nextFrame(){
	float animT = 1.0f - ((ofGetFrameNum() % numFrame) / (float)numFrame);
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	sample.getTexture().bind();
	screenShader.begin();
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screen.draw();
	screenShader.end();
	sample.getTexture().unbind();

	sampleCroped.getTexture().bind();
	billboardShader.begin();
	billboardShader.setUniform1f("u_size", animT);
	billboards.draw();
	billboardShader.end();
	sampleCroped.getTexture().unbind();
	ofDisablePointSprites();
	ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::draw(){

	capture.draw(0, 0, 1920, 1080);

	// debug & info
	if(showInfo){
		sampleCroped.draw(4, 50);
		string frameRate = ofToString(ofGetFrameRate(), 2) + "\n";
		string loopCount = ofToString(loop) + "/" + ofToString(numLoop) + "\n";
		//string particleCount = "Particle Count: " + ofToString(NUM_BILLBOARDS);
		ofSetColor(255);
		ofDrawBitmapStringHighlight(frameRate + loopCount, 30, 30);
	}
	if(isRecording && ofGetFrameNum() % 10 == 0){
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofDrawCircle(10, 10, 0, 15);
		ofPopStyle();

	}
}
void ofApp::keyPressed(int key){
	if(key == 'r' || key == 'R'){
		isRecording = !isRecording;
	}
	if(key == 'i' || key == 'I'){
		showInfo = !showInfo;
	}
	if(key == 'p' || key == 'P'){
	}
}