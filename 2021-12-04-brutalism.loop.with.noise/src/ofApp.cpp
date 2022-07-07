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
void ofApp::loadSound(){
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
	for(int i = 0; i < longSndNum; i++){
		string longSoundFile = longAudioDir.getPath(i);
		longSound[i].loadSound(longSoundFile);
		longSound[i].setVolume(0.5);
		longSound[i].stop();
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

	ofSetFrameRate(24);
	ofSetWindowShape(screenWidth, screenHeight);
	center = ofVec2f(screenWidth / 2, screenHeight / 2);

	original.load("images/1080x1920/" + imageSource);
	stepFrame[0] = original;
	//loadSound();

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
	settings.numThreads = 12;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);

	nextMove();
	//playSound("long");
}

//--------------------------------------------------------------
void ofApp::nextMove(){
	for(int i = 0; i < MOVE_PER_ITERATION; i++){
		c[t].isVertical[i] = ofRandomuf() > 0.5;
		c[t].goForward[i] = ofRandomuf() > 0.5;
		c[t].size[i] = ceil((ofRandomuf() * (c[t].isVertical[i] ? screenWidth : screenHeight)) * 0.02);
		c[t].stepSize[i] = ceil((ofRandomuf() * (c[t].isVertical[i] ? screenHeight : screenWidth)) * 0.3);
		c[t].distance[i] = 0;
		c[t].spreadSize[i] = 12 + ofRandom(24);
		c[t].noiseScale[i] = 250 + ofRandom(1750);
		float sampleWidth = c[t].isVertical[i] ? c[t].size[i] : c[t].stepSize[i];
		float sampleHeight = c[t].isVertical[i] ? c[t].stepSize[i] : c[t].size[i];
		c[t].rect[i].setX(ofRandom(0, screenWidth - sampleWidth));
		c[t].rect[i].setY(ofRandom(0, screenHeight - sampleHeight));
		c[t].rect[i].setWidth(sampleWidth);
		c[t].rect[i].setHeight(sampleHeight);

		stepFrame[t].getPixels().cropTo(
			c[t].crop[i],
			c[t].rect[i].getX(),
			c[t].rect[i].getY(),
			c[t].rect[i].getWidth(),
			c[t].rect[i].getHeight()
			);

	}
}
//--------------------------------------------------------------
void ofApp::resetBillboard(){
	for(int i = 0; i < NUM_BILLBOARDS; i++){
		int r = (int)ofRandom(0, MOVE_PER_ITERATION);
		size_t x, y;
		float weight = ofRandom(1);
		billboardVels[i] = {ofRandomf(), -0.5 + (-weight / 2), ofRandomf()};
		billboardSizeTarget[i] = 8 * floor(ofRandom(32 * weight));

		int way = c[t].goForward[r] ? 1 : -1;
		int distance = ceil(ofRandom(c[t].distance[r] + 1));

		int distX = c[t].isVertical[r] ? c[t].rect[r].getWidth() * distance : c[t].rect[r].getHeight();
		int distY = c[t].isVertical[r] ? c[t].rect[r].getWidth() : c[t].rect[r].getHeight() * c[t].distance[r];

		x = c[t].rect[r].getX() + (ofRandom(2) * way * distX);
		y = c[t].rect[r].getY() + (ofRandom(2) * way * distY);
		x += glm::cos(ofRandom(1) * glm::two_pi <float>()) * c[t].spreadSize[r];
		y += glm::sin(ofRandom(1) * glm::two_pi <float>()) * c[t].spreadSize[r];

		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			stepFrame[t == ITERATIONS ? t : t + 1].getColor(x, y));
	}
}

//--------------------------------------------------------------
void ofApp::moveBillboard(){
	float t =
		1.0f - ease((ofGetFrameNum() % FRAME_PER_ITERATION) / static_cast <float>(FRAME_PER_ITERATION), 5);

	for(int i = 0; i < NUM_BILLBOARDS; i++){

		glm::vec3 vec(0, t + ofSignedNoise(billboards.getVertex(i).x, billboards.getVertex(i).y, 0, 0), 0);
		billboardVels[i] += vec;
		// billboardVels[i].y *= 1.01f;
		billboards.getVertices()[i] += billboardVels[i];
		billboardVels[i] *= 0.94f;
		billboards.setNormal(
			i, glm::vec3(12 + billboardSizeTarget[i] * ofNoise(t + i), 0, 0));
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();
	// DESTRUCTURING
	if(playingForward){
		for(int i = 0; i < MOVE_PER_ITERATION; i++){
			int way = c[t].goForward[i] ? 1 : -1;
			int distX = c[t].isVertical[i] ? c[t].distance[i] : 0;
			int distY = c[t].isVertical[i] ? 0 : c[t].distance[i];

			ofVec2f displace(
				c[t].rect[i].getX() + way * distX * c[t].rect[i].getWidth(),
				c[t].rect[i].getY() + way * distY * c[t].rect[i].getHeight()
				);
			float spreadNoise = ofNoise(displace.x, displace.y, t);
			displace.x += glm::cos(spreadNoise + (playingForward ? 0 : glm::two_pi <float>())) * c[t].spreadSize[i];
			displace.y += glm::sin(spreadNoise + (playingForward ? 0 : glm::two_pi <float>())) * c[t].spreadSize[i];

			if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
				// Random gaps
				if(ofRandom(1.0f) < 0.8){
					ofPixels mixedFrame = c[t].crop[i];
					for(int y = 0; y < c[t].crop[i].getHeight(); y++){
						for(int x = 0; x < c[t].crop[i].getWidth(); x++){

							ofColor sampleColor = stepFrame[t].getColor(
								displace.x + x,
								displace.y + y
								);
							ofColor sliceColor = c[t].crop[i].getColor(x, y);

							float noise = ease(
								ofNoise((displace.x + x) / c[t].noiseScale[i],
										(displace.y + y) / c[t].noiseScale[i],
										t * i),
								500);
							ofColor between = sampleColor.lerp(sliceColor, noise);
							mixedFrame.setColor(x, y, between);
						}
					}

					mixedFrame.pasteInto(stepFrame[t].getPixels(), displace.x, displace.y);
				}

				stepFrame[t].update();
			}
			c[t].distance[i]++;
		}

	}
	// RESTRUCTURING
	else{
		moveBillboard();
	}

	/* if(ofGetFrameNum() % 16 == 0){
		playSound("short");
	} */

	capture.begin();
	ofClear(0, 255);
	nextFrame();
	capture.end();

	if(isRecording){
		recorder.save(capture.getTexture());
	}

	if(ofGetFrameNum() > 0 && (ofGetFrameNum() % FRAME_PER_ITERATION) == 0){
		if(playingForward){
			if(t < ITERATIONS){
				t++;
				stepFrame[t] = stepFrame[t - 1];
				nextMove();
			}else{
				playingForward = false;
				stepFrame[0] = original;
			}
		}else{

			if(t > 0){
				resetBillboard();
				t--;
			}else{
				std::cout << "Shutdown" << endl;
				isRecording = false;
				ofExit();
			}
		}
		std::cout << ofToString(t) << "/" << ofToString(ITERATIONS) << " move " << (playingForward ? "forward" : "backward") << endl;
	}
}
//--------------------------------------------------------------

void ofApp::nextFrame(){
	float animT = 1.0f - (ofGetFrameNum() % FRAME_PER_ITERATION) / static_cast <float>(FRAME_PER_ITERATION);
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	stepFrame[t].getTexture().bind();
	screenShader.begin();
	screenShader.setUniform1f("u_time", animT / 100.0f);
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screen.draw();
	screenShader.end();
	stepFrame[t].getTexture().unbind();


	billboardShader.begin();
	billboardShader.setUniform1f("u_size", ease(animT));
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
		ofSetColor(255);
	}
	if(isRecording && ofGetFrameNum() % 16 == 0){
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
	//ofSoundShutdown();

}
