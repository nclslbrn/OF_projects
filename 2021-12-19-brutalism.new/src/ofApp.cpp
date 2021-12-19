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
void ofApp::loadSound(bool verbose = false){
	shortSndNum = shortAudioDir.listDir("audio-sample/short");
	longSndNum = longAudioDir.listDir("audio-sample/long");
	shortSound = new ofSoundPlayer[shortSndNum];
	longSound = new ofSoundPlayer[longSndNum];
	for(int i = 0; i < shortSndNum; i++){
		string shortSoundFile = shortAudioDir.getPath(i);
		shortSound[i].loadSound(shortSoundFile);
		shortSound[i].setVolume(0.65);
		shortSound[i].stop();
	}
	if(verbose){
		std::cout << shortSndNum << " shorts audio samples loaded " << endl;
	}

	for(int i = 0; i < longSndNum; i++){
		string longSoundFile = longAudioDir.getPath(i);
		longSound[i].loadSound(longSoundFile);
		longSound[i].setVolume(0.5);
		longSound[i].stop();
	}
	if(verbose){
		std::cout << longSndNum << " longs audio samples loaded " << endl;
	}

}
//--------------------------------------------------------------
void ofApp::playSound(string soundType){
	if(soundType == "long"){
		if(longSound[currLongSound].isPlaying()){
			longSound[currLongSound].stop();
		}
		currLongSound = (int)ofRandom(0, longSndNum);
		if(longSound[currLongSound].isLoaded()){
			std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
			longSound[currLongSound].play();
		}
	}else{

		currShortSound = (int)ofRandom(0, shortSndNum);
		if(shortSound[currShortSound].isLoaded()){
			shortSound[currShortSound].play();
		}
	}
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(25);
	ofSetWindowShape(screenWidth, screenHeight);

	imgNum = sizeof(imageSource) / sizeof(imageSource[0]);
	image = new ofImage[imgNum];

	for(int i = 0; i < imgNum; i++){
		string imageFile = (alreadyProcessedImage ? "processed-image/" : "images/2880x1620/") + imageSource[i];
		image[i].load(imageFile);
	}
	// loadSound(true);

	screenShader.load("shaders/Screen");
	billboardShader.load("shaders/Billboard");

	screen.set(screenWidth, screenHeight);
	screen.setPosition(center.x, center.y, 0);
	screen.setScale(1, -1, 1);
	screen.mapTexCoords(0, 0, screenWidth, screenHeight);

	// of/exmaples/billboardExemple
	billboards.getVertices().resize(NUM_BILLBOARDS);
	billboards.getColors().resize(NUM_BILLBOARDS);
	billboards.getNormals().resize(NUM_BILLBOARDS, glm::vec3(0));
	billboards.setUsage(GL_DYNAMIC_DRAW);
	billboards.setMode(OF_PRIMITIVE_POINTS);

	// of/addons/ofxTextureRecorder/example
	capture.allocate(screenWidth, screenHeight, GL_RGB);
	ofxTextureRecorder::Settings settings(capture.getTexture());
	settings.w = screenWidth;
	settings.h = screenHeight;
	settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
	//settings.pixelFormat = OF_PIXELS_RGB;
	settings.numThreads = 16;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);

	currImg = 0;
	loadSound(false);
	nextMove();
}

//--------------------------------------------------------------
void ofApp::nextMove(){

	for(int i = 0; i < NUM_MOVE; i++){
		m[i].isVertical = ofRandomuf() > 0.5;
		m[i].isStopped = false;
		m[i].goForward = ofRandomuf() > 0.5;
		m[i].size = ceil((ofRandomuf() * (m[i].isVertical ? screenWidth : screenHeight)) * 0.015);
		m[i].stepSize = ceil((ofRandomuf() * (m[i].isVertical ? screenHeight : screenWidth)) * 0.17);
		m[i].distance = 0;
		m[i].spreadSize = 12 + ofRandom(24);
		m[i].noiseScale = 250 + ofRandom(1750);
		ofVec2f sampleSize(
			m[i].isVertical ? m[i].size : m[i].stepSize,
			m[i].isVertical ? m[i].stepSize : m[i].size
			);
		m[i].rect.setX(ofRandom(0, screenWidth - sampleSize.x));
		m[i].rect.setY(ofRandom(0, screenHeight - sampleSize.y));
		m[i].rect.setWidth(sampleSize.x);
		m[i].rect.setHeight(sampleSize.y);
		image[currImg].getPixels().cropTo(
			m[i].crop,
			m[i].rect.getX(),
			m[i].rect.getY(),
			m[i].rect.getWidth(),
			m[i].rect.getHeight()
			);
	}

	for(int i = 0; i < NUM_BILLBOARDS; i++){
		int rm = (int)ofRandom(0, NUM_MOVE);
		size_t x = m[rm].rect.getX() + (ofRandom(0, m[rm].rect.getWidth()));
		size_t y = m[rm].rect.getY() + (ofRandom(0, m[rm].rect.getHeight()));

		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			image[currImg].getColor(x, y));
		billboardSizeTarget[i] = m[rm].stepSize * ofRandomuf();
	}
}
//--------------------------------------------------------------
void ofApp::move(){
	float t = 1.0f - ease((ofGetFrameNum() % frameNum) / static_cast <float>(frameNum), 5);
	for(int i = 0; i < NUM_MOVE; i++){
		if(m[i].isStopped){
			return;
		}
		int way = m[i].goForward ? 1 : -1;
		int distX = m[i].isVertical ? m[i].distance : 0;
		int distY = m[i].isVertical ? 0 : m[i].distance;
		ofVec2f displace(
			m[i].rect.getX() + way * distX * m[i].rect.getWidth(),
			m[i].rect.getY() + way * distY * m[i].rect.getHeight()
			);
		float spreadNoise = ofNoise(displace.x, displace.y, t);
		displace.x += glm::cos(spreadNoise) * m[i].spreadSize;
		displace.y += glm::sin(spreadNoise) * m[i].spreadSize;


		if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
			m[i].distance++;
			m[i].crop.pasteInto(image[currImg].getPixels(), displace.x, displace.y);
			image[currImg].update();
		}else{
			m[i].isStopped = true;
		}
	}


}
//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	if(ofGetFrameNum() == 0 || (int)ofGetFrameNum() % frameNum == 0){
		if(ofGetFrameNum() != 0 && loop < loopNum){
			loop++;
		}else{
			if(ofGetFrameNum() != 0 && currImg < imgNum - 1){
				currImg++;
				nextMove();
			}else{
				// If exery image has been processed one time quit the program
				if(ofGetFrameNum() != 0){
					// Quit programm
					isRecording = false;
					ofExit();
				}
			}
			std::cout << "New image "
					  << imageSource[currImg] << " "
					  << ofToString(currImg) << "/" << ofToString(imgNum - 1) << endl;
			playSound("long");

			loop = 0;
		}
		// how many frame per loop
		frameNum = 4 * ceil(ofRandomuf() * 16);
		playSound("short");
		move();

	}
	float t = 1.0f - ease((ofGetFrameNum() % frameNum) / static_cast <float>(frameNum), 5);

	//playSound("short");
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
//--------------------------------------------------------------
void ofApp::nextFrame(){
	float animT = 1.0f - ((ofGetFrameNum() % frameNum) / (float)frameNum);
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	image[currImg].getTexture().bind();
	screenShader.begin();
	screenShader.setUniform1f("u_time", animT / 100.0f);
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screenShader.setUniform1f("u_size", ease(animT, 5));
	screen.draw();
	screenShader.end();
	image[currImg].getTexture().unbind();

	billboardShader.begin();
	billboardShader.setUniform1f("u_size", animT);
	billboards.draw();
	billboardShader.end();

	ofDisablePointSprites();
	ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::draw(){

	capture.draw(0, 0, 1920, 1080);

	// debug & info
	if(showInfo){
		string frameRate = ofToString(ofGetFrameRate(), 2) + " FPS \n";
		string loopCount = ofToString(loop) + "/" + ofToString(loopNum) + " LOOP \n";
		string imageNum = ofToString(currImg) + "/" + ofToString(imgNum) + " IMG \n";
		//string particleCount = "Particle Count: " + ofToString(NUM_BILLBOARDS);
		ofSetColor(255);
		ofDrawBitmapStringHighlight(frameRate + loopCount + imageNum, 30, 30);
	}
	if(isRecording && ofGetFrameNum() % 10 == 0){
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofDrawCircle(10, 10, 0, 15);
		ofPopStyle();

	}
}
//--------------------------------------------------------------
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
//--------------------------------------------------------------
void ofApp::exit(){
	// Stop sound
	ofSoundStopAll();
	ofSoundShutdown();

	// Save processed image to reuse
	for(int i = 0; i < imgNum; i++){
		string imageOut = imageSource[i];
		//std::cout << "Image n°" << ofToString(i) << " " << ofToString(imageFile) << endl;
		image[i].save("processed-image/" + imageOut);
	}

}
