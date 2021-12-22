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
void ofApp::fadeOutSound(string soundType){
	float fade = 0.98;
	if(soundType == "long" || soundType == "both"){
		float volume = longSound[currLongSound].getVolume();
		volume *= fade;
		longSound[currLongSound].setVolume(volume);
	}
	if(soundType == "short" || soundType == "both"){
		float volume = shortSound[currShortSound].getVolume();
		volume *= fade;
		shortSound[currShortSound].setVolume(volume);
	}
}
//--------------------------------------------------------------
void ofApp::playSound(string soundType, bool verbose = false){
	if(soundType == "long"){
		if(longSound[currLongSound].isPlaying()){
			longSound[currLongSound].stop();
		}
		currLongSound = (int)ofRandom(0, longSndNum);
		if(longSound[currLongSound].isLoaded()){
			longSound[currLongSound].setVolume(0.5);
			longSound[currLongSound].play();
			if(verbose){
				std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
			}
		}
	}else{
		/*
		if(shortSound[currShortSound].isPlaying()){
			shortSound[currShortSound].stop();
		}
		*/
		currShortSound = (int)ofRandom(0, shortSndNum);
		if(shortSound[currShortSound].isLoaded()){
			shortSound[currShortSound].setVolume(0.65);
			shortSound[currShortSound].play();
			if(verbose){
				std::cout << "Playing short audio sample " << ofToString(currShortSound) << "/" << ofToString(shortSndNum - 1) << endl;
			}
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
	playSound("long");
	playSound("short");

}

//--------------------------------------------------------------
void ofApp::nextMove(){

	for(int i = 0; i < NUM_MOVE; i++){
		m[i].isVertical = ofRandomuf() > 0.5;
		m[i].isStopped = false;
		m[i].goForward = ofRandomuf() > 0.5;
		m[i].size = ceil((ofRandomuf() * (m[i].isVertical ? screenWidth : screenHeight)) * 0.015);
		m[i].stepSize = ceil((ofRandomuf() * (m[i].isVertical ? screenHeight : screenWidth)) * 0.2);
		m[i].distance = 0;
		m[i].spreadSize = 8.0f + ofRandom(12);
		m[i].noiseScale = ofRandom(500) + 3000.0f;
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
		float weight = 0.5f + ofRandom(0.5f);
		size_t x = m[rm].rect.getX() + (ofRandom(0, m[rm].rect.getWidth()));
		size_t y = m[rm].rect.getY() + (ofRandom(0, m[rm].rect.getHeight()));

		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			image[currImg].getColor(x, y));

		billboardVels[i] = {ofRandomf(), ofRandomf(), ofRandomf()};
		billboardSizeTarget[i] = 72.0f + ofRandom(4);

		/* billboardVels[i] = {
			sin(weight) * sin(n),
			cos(n),
			cos(weight) * sin(n)
		};
		billboardSizeTarget[i] = 92.0f + floor(4 * weight); */
	}

	frameNum = 72 + ceil(ofRandomuf() * 72);

}
//--------------------------------------------------------------
void ofApp::move(float animT = 0.0f){
	for(int i = 0; i < NUM_MOVE; i++){
		if(!m[i].isStopped){
			int way = m[i].goForward ? 1 : -1;
			int distX = m[i].isVertical ? m[i].distance : 0;
			int distY = m[i].isVertical ? 0 : m[i].distance;
			ofVec2f displace(
				m[i].rect.getX() + way * distX * m[i].rect.getWidth(),
				m[i].rect.getY() + way * distY * m[i].rect.getHeight()
				);
			float spreadNoise = ofNoise(displace.x, displace.y, animT);
			displace.x += glm::cos(spreadNoise) * m[i].spreadSize;
			displace.y += glm::sin(spreadNoise) * m[i].spreadSize;


			if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
				// Random gaps
				if(ofRandom(1.0f) < 0.75){
					ofPixels mixed = m[i].crop;
					for(int y = 0; y < m[i].crop.getHeight(); y++){
						for(int x = 0; x < m[i].crop.getWidth(); x++){
							if(
								displace.x + x > 0 &&
								displace.y + y > 0 &&
								displace.x + x < screenWidth &&
								displace.y + y < screenHeight
								){

								ofColor sampleColor = image[currImg].getColor(
									displace.x + x,
									displace.y + y
									);
								ofColor sliceColor = m[i].crop.getColor(x, y);

								float noise = ofNoise(
									(displace.x + x) / m[i].noiseScale,
									(displace.y + y) / m[i].noiseScale,
									loop * i) > 0.5 ? 1.0 : 0.0;
								ofColor between = sampleColor.lerp(sliceColor, noise);
								mixed.setColor(x, y, between);
							}
						}
					}
					mixed.pasteInto(image[currImg].getPixels(), displace.x, displace.y);
					image[currImg].update();
				}
				m[i].distance++;
			}else{
				m[i].isStopped = true;
			}
		}
	}
	for(int i = 0; i < NUM_BILLBOARDS; i++){
		float vec = ofNoise(animT + billboards.getVertex(i).x, animT + billboards.getVertex(i).y);
		billboardVels[i] *= glm::vec3(0.5f + vec, 0.5f + vec, 0.5f + vec);
		//billboardVels[i] *= 1.01f;
		billboards.getVertices()[i] += billboardVels[i];
		//billboardVels[i] *= 0.74f;
		billboards.setNormal(
			i, glm::vec3(billboardSizeTarget[i] * ofNoise(animT + i), 0, 0));
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	if(ofGetFrameNum() == 0 || (int)ofGetFrameNum() % frameNum == 0){
		if(ofGetFrameNum() != 0 && loop < loopNum){
			loop++;
			playSound("short");
			nextMove();

		}else{
			if(ofGetFrameNum() != 0 && currImg < imgNum - 1){
				currImg++;
				nextMove();
				playSound("long");

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

			loop = 0;
		}
		// how many frame per loop
	}
	float t = 1.0f - (ease((ofGetFrameNum() % frameNum) / static_cast <float>(frameNum), 5));

	capture.begin();
	ofClear(0, 255);
	nextFrame(t);
	capture.end();

	if(isRecording){
		if(ofGetFrameNum() > 0){
			recorder.save(capture.getTexture());
		}
	}
	move(t);
	//fadeOutSound("both");
}
//--------------------------------------------------------------
void ofApp::nextFrame(float animT = 0.0f){
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
