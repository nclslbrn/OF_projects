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

	ofSetFrameRate(25);
	ofSetWindowShape(screenWidth, screenHeight);
	center = ofVec2f(screenWidth / 2, screenHeight / 2);
	//shortSndNum = shortAudioDir.listDir("audio-sample/short");
	//longSndNum = longAudioDir.listDir("audio-sample/long");
	//shortSound = new ofSoundPlayer[shortSndNum];
	//longSound = new ofSoundPlayer[longSndNum];
	original.load("images/1080x1080/" + imageSource);
	copy = original;
	/* //std::cout << "Long audio " + ofToString(longSndNum) << endl;
	for(int i = 0; i < shortSndNum; i++){
		string shortSoundFile = shortAudioDir.getPath(i);
		shortSound[i].loadSound(shortSoundFile);
		shortSound[i].setVolume(0.65);
		shortSound[i].stop();
	}
	std::cout << shortSndNum << " shorts audio samples loaded " << endl;
	for(int i = 0; i < longSndNum; i++){
		string longSoundFile = longAudioDir.getPath(i);
		longSound[i].loadSound(longSoundFile);
		longSound[i].setVolume(0.65);
		longSound[i].stop();
	}
	std::cout << longSndNum << " longs audio samples loaded " << endl; */

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

	/* currLongSound = (int)ofRandom(0, longSndNum);
	if(longSound[currLongSound].isLoaded()){
		std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
		longSound[currLongSound].play();
	} */
	// how many frame per loop
	// frameNum = 6 * ceil(ofRandomuf() * 16);
	nextMove();

}

//--------------------------------------------------------------
void ofApp::nextMove(){
	// Graphics

	for(int h = 0; h < MOVE_PER_ITERATION; h++){
		c.isVertical[h] = ofRandomuf() > 0.5;
		c.goForward[h] = ofRandomuf() > 0.5;
		c.size[h] = ceil((ofRandomuf() * (c.isVertical[h] ? screenWidth : screenHeight)) * 0.02f);
		c.stepSize[h] = ceil((ofRandomuf() * (c.isVertical[h] ? screenHeight : screenWidth)) * 0.3f);
		c.distance[h] = 0;
		float sampleWidth = c.isVertical[h] ? c.size[h] : c.stepSize[h];
		float sampleHeight = c.isVertical[h] ? c.stepSize[h] : c.size[h];
		c.rect[h].setX(ofRandom(0, screenWidth - sampleWidth));
		c.rect[h].setY(ofRandom(0, screenHeight - sampleHeight));
		c.rect[h].setWidth(sampleWidth);
		c.rect[h].setHeight(sampleHeight);

		copy.getPixels().cropTo(
			c.crop[h],
			c.rect[h].getX(),
			c.rect[h].getY(),
			c.rect[h].getWidth(),
			c.rect[h].getHeight()
			);

/*
		// Let sound finnish to play to its end
		if(shortSound[currShortSound].isPlaying()){
			shortSound[currShortSound].stop();
		}
		currShortSound = (int)ofRandom(0, shortSndNum);
		if(shortSound[currShortSound].isLoaded()){
			shortSound[currShortSound].play();
		} */
	}
	for(int i = 0; i < NUM_BILLBOARDS; i++){
		int randRect = (int)ofRandom(0, MOVE_PER_ITERATION);
		size_t x = c.rect[randRect].getX() + (ofRandom(0, c.rect[randRect].getWidth()));
		size_t y = c.rect[randRect].getY() + (ofRandom(0, c.rect[randRect].getHeight()));
		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			copy.getColor(x, y));
		billboardSizeTarget[i] = 8 * floor(ofRandom(8));
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	/* if(shortSound[currShortSound].isLoaded() && !shortSound[currShortSound].isPlaying()){
		shortSound[currShortSound].play();
	} */

	for(int h = 0; h < MOVE_PER_ITERATION; h++){
		ofVec2f displace(
			c.rect[h].getX() + (c.goForward[h] ? 1 : -1) * (c.isVertical[h] ? c.distance[h] : 0) * c.rect[h].getWidth(),
			c.rect[h].getY() + (c.goForward[h] ? 1 : -1) * (c.isVertical[h] ? 0 : c.distance[h]) * c.rect[h].getHeight()
			);

		if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
			if(playingForward){
				c.crop[h].pasteInto(copy.getPixels(), displace.x, displace.y);
			}else{
				original.getPixels().cropTo(
					c.crop[h],
					displace.x,
					displace.y,
					c.rect[h].getWidth(),
					c.rect[h].getHeight()
					);
				c.crop[h].pasteInto(copy.getPixels(), displace.x, displace.y);
			}
			copy.update();
		}
		if(playingForward){
			c.distance[h]++;
		}else{
			c.distance[h]--;
		}
	}


	float t =
		1.0f - ease((ofGetFrameNum() % (FRAME_PER_ITERATION / 2)) / static_cast <float>(FRAME_PER_ITERATION / 2), 5);

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
		// if(ofGetFrameNum() > 0){
		recorder.save(capture.getTexture());
		//}
	}
	if(ofGetFrameNum() > 0 && (ofGetFrameNum() % FRAME_PER_ITERATION) == 0){
		if(playingForward){
			if(nIter < ITERATIONS){
				nIter++;
			}else{
				playingForward = false;
			}
		}else{
			if(nIter > 0){
				nIter--;
			}else{
				isRecording = false;
				ofExit();
			}
		}
		std::cout << ofToString(nIter) << "/" << ofToString(ITERATIONS) << " move " << (playingForward ? "forward" : "backward") << endl;
	}
}
//--------------------------------------------------------------

void ofApp::nextFrame(){
	float animT = 1.0f - ((ofGetFrameNum() % (FRAME_PER_ITERATION / 2)) / (float)FRAME_PER_ITERATION / 2);
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	copy.getTexture().bind();
	screenShader.begin();
	screenShader.setUniform1f("u_time", animT / 100.0f);
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screen.draw();
	screenShader.end();
	copy.getTexture().unbind();

	billboardShader.begin();
	billboardShader.setUniform1f("u_size", animT);
	billboards.draw();
	billboardShader.end();

	ofDisablePointSprites();
	ofDisableAlphaBlending();
}
//--------------------------------------------------------------
void ofApp::draw(){

	capture.draw(0, 0, 1080, 1080);

	// debug & info
	if(showInfo){
		string frameRate = ofToString(ofGetFrameRate(), 2) + " FPS \n";
		//string particleCount = "Particle Count: " + ofToString(NUM_BILLBOARDS);
		ofSetColor(255);
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


}
