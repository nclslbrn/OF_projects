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
void ofApp::setup(){
	screenWidth = 2880;
	screenHeight = 1620;
	loop = 0;
	loopNum = 10;
	isRecording = false;
	showInfo = false;
	ofSetFrameRate(25);
	ofSetWindowShape(screenWidth, screenHeight);
	center = ofVec2f(screenWidth / 2, screenHeight / 2);
	imgNum = imageDir.listDir("images/2880x1620/");
	shortSndNum = shortAudioDir.listDir("audio-sample/short");
	longSndNum = longAudioDir.listDir("audio-sample/long");

	image = new ofImage[imgNum];
	shortSound = new ofSoundPlayer[shortSndNum];
	longSound = new ofSoundPlayer[longSndNum];


	for(int i = 0; i < imgNum; i++){
		string imageFile = imageDir.getPath(i);
		//std::cout << "Image n°" << ofToString(i) << " " << ofToString(imageFile) << endl;
		image[i].load(imageFile);
	}
	std::cout << ofToString(imgNum) << " images loaded" << endl;
	//std::cout << "Long audio " + ofToString(longSndNum) << endl;
	for(int i = 0; i < shortSndNum; i++){
		string shortSoundFile = shortAudioDir.getPath(i);
		// std::cout << shortAudioDir.getPath(si) << endl;
		shortSound[i].setMultiPlay(true);
		shortSound[i].setLoop(true);
		shortSound[i].loadSound(shortSoundFile);
		shortSound[i].stop();
	}
	std::cout << shortSndNum << " shorts audio samples loaded " << endl;
	for(int i = 0; i < longSndNum; i++){
		string longSoundFile = longAudioDir.getPath(i);
		// std::cout << longAudioDir.getPath(si) << endl;
		// longSound[i].setMultiPlay(true);
		longSound[i].loadSound(longSoundFile);
		// longSound[i].setLoop(true);
		longSound[i].setVolume(0.75);
		longSound[i].stop();
	}
	std::cout << longSndNum << " longs audio samples loaded " << endl;

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
	settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
	settings.numThreads = 4;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);


	currImg = 0;
}

//--------------------------------------------------------------
void ofApp::nextMove(){
	// Graphics
	isVertical = ofRandomuf() > 0.5;
	size = (int)(1 + (ofRandomuf() * (isVertical ? screenHeight : screenWidth) * 0.015));
	stepSize = (int)(1 + ofRandomuf() * 320);
	goForward = ofRandomuf() > 0.5;
	d = 0;
	float sampleWidth = isVertical ? size : stepSize;
	float sampleHeight = isVertical ? stepSize : size;
	rect.setX(ofRandom(0, screenWidth - sampleWidth));
	rect.setY(ofRandom(0, screenHeight - sampleHeight));
	rect.setWidth(sampleWidth);
	rect.setHeight(sampleHeight);

	sampleCroped = image[currImg];
	sampleCroped.crop(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
	image[currImg].getPixels().cropTo(crop, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

	for(int i = 0; i < NUM_BILLBOARDS; i++){
		size_t x = rect.getX() + (ofRandom(0, sampleWidth));
		size_t y = rect.getY() + (ofRandom(0, sampleHeight));

		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			image[currImg].getColor(x, y));
		billboardSizeTarget[i] = stepSize * ofRandomuf();
	}
	// Sounds
	/* if(shortSound[currShortSound].isPlaying()){
		shortSound[currShortSound].stop();
	} */

	currShortSound = (int)ofRandom(0, shortSndNum);
	if(shortSound[currShortSound].isLoaded()){
		shortSound[currShortSound].play();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	if(ofGetFrameNum() == 0 || (int)ofGetFrameNum() % frameNum == 0){

		if(ofGetFrameNum() != 0 && loop < loopNum){

			loop++;

		}else{

			if(currImg < imgNum - 1){
				currImg++;

			}else{
				currImg = 0;

			}
			std::cout << "Switch image " << ofToString(currImg) << "/" << ofToString(imgNum - 1) << endl;

			if(longSound[currLongSound].isPlaying()){
				longSound[currLongSound].stop();
			}
			currLongSound = (int)ofRandom(0, longSndNum);
			if(longSound[currLongSound].isLoaded()){
				std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
				longSound[currLongSound].play();
			}
			loop = 0;
		}

		frameNum = 6 * ceil(ofRandomuf() * 8);
		nextMove();

	}
	if(
		shortSound[currShortSound].isLoaded() &&
		!shortSound[currShortSound].isPlaying() &&
		ofGetFrameNum() % (frameNum / 6) == 0
		){
		shortSound[currShortSound].play();
	}

	ofVec2f displace(
		rect.getX() + (goForward ? 1 : -1) * (isVertical ? d : 0) * rect.getWidth(),
		rect.getY() + (goForward ? 1 : -1) * (isVertical ? 0 : d) * rect.getHeight()
		);

	if(displace.x < 0){
		displace.x += screenWidth;
	}
	if(displace.y < 0){
		displace.y += screenHeight;
	}
	if(displace.x > screenWidth){
		displace.x = 1.0f * (static_cast <int>(round(displace.x)) % screenWidth);
	}
	if(displace.y > screenHeight){
		displace.y = 1.0f * (static_cast <int>(round(displace.y)) % screenHeight);
	}

	crop.pasteInto(image[currImg].getPixels(), displace.x, displace.y);
	image[currImg].update();
	d++;

	float t =
		1.0f - ease((ofGetFrameNum() % frameNum) / static_cast <float>(frameNum), 5);

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
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screen.draw();
	screenShader.end();
	image[currImg].getTexture().unbind();

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
	ofSoundStopAll();
	ofSoundShutdown();
}