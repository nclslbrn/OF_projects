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
		currLongSound = (int)ofRandom(0, longSndNum);
		if(longSound[currLongSound].isLoaded()){
			std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
			longSound[currLongSound].play();
		}
	}else{
		/* if(shortSound[currShortSound].isPlaying()){
			shortSound[currShortSound].stop();
		} */
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

	original.load("images/1080x1080/" + imageSource);
	stepFrame[0] = original;
	loadSound();

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
	settings.numThreads = 12;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);

	nextMove();
	resetBillboard();

}

//--------------------------------------------------------------
void ofApp::nextMove(){
	for(int h = 0; h < MOVE_PER_ITERATION; h++){
		c[nIter].isVertical[h] = ofRandomuf() > 0.5;
		c[nIter].goForward[h] = ofRandomuf() > 0.5;
		c[nIter].size[h] = ceil((ofRandomuf() * (c[nIter].isVertical[h] ? screenWidth : screenHeight)) * 0.015f);
		c[nIter].stepSize[h] = ceil((ofRandomuf() * (c[nIter].isVertical[h] ? screenHeight : screenWidth)) * 0.2f);
		c[nIter].distance[h] = 0;
		float sampleWidth = c[nIter].isVertical[h] ? c[nIter].size[h] : c[nIter].stepSize[h];
		float sampleHeight = c[nIter].isVertical[h] ? c[nIter].stepSize[h] : c[nIter].size[h];
		c[nIter].rect[h].setX(ofRandom(0, screenWidth - sampleWidth));
		c[nIter].rect[h].setY(ofRandom(0, screenHeight - sampleHeight));
		c[nIter].rect[h].setWidth(sampleWidth);
		c[nIter].rect[h].setHeight(sampleHeight);

		stepFrame[nIter].getPixels().cropTo(
			c[nIter].crop[h],
			c[nIter].rect[h].getX(),
			c[nIter].rect[h].getY(),
			c[nIter].rect[h].getWidth(),
			c[nIter].rect[h].getHeight()
			);

	}
	playSound("long");
}
//--------------------------------------------------------------
void ofApp::resetBillboard(){
	for(int i = 0; i < NUM_BILLBOARDS; i++){
		int r = (int)ofRandom(0, MOVE_PER_ITERATION);
		size_t x, y;
		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboardSizeTarget[i] = (playingForward ?  6 : 4) * floor(ofRandom(8));
		if(playingForward){
			x = c[nIter].rect[r].getX() + (ofRandom(0, c[nIter].rect[r].getWidth()));
			y = c[nIter].rect[r].getY() + (ofRandom(0, c[nIter].rect[r].getHeight()));
		}else{
			int way = c[nIter].goForward[r] ? 1 : -1;
			int distX = c[nIter].isVertical[r] ? c[nIter].rect[r].getWidth() * c[nIter].distance[r] : c[nIter].rect[r].getHeight();
			int distY = c[nIter].isVertical[r] ? c[nIter].rect[r].getWidth() : c[nIter].rect[r].getHeight() * c[nIter].distance[r];
			x = c[nIter].rect[r].getX() + (ofRandom(1) * way * distX);
			y = c[nIter].rect[r].getY() + (ofRandom(1) * way * distY);
		}
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			stepFrame[nIter].getColor(x, y));
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
	moveBillboard();

	for(int h = 0; h < MOVE_PER_ITERATION; h++){
		int way = c[nIter].goForward[h] ? 1 : -1;
		int distX = c[nIter].isVertical[h] ? c[nIter].distance[h] : 0;
		int distY = c[nIter].isVertical[h] ? 0 : c[nIter].distance[h];
		ofVec2f displace(
			c[nIter].rect[h].getX() + way * distX * c[nIter].rect[h].getWidth(),
			c[nIter].rect[h].getY() + way * distY * c[nIter].rect[h].getHeight()
			);

		if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
			if(playingForward){
				ofPixels mixedFrame = c[nIter].crop[h];

				for(int y = 0; y < c[nIter].crop[h].getHeight(); y++){
					for(int x = 0; x < c[nIter].crop[h].getWidth(); x++){

						ofColor sampleColor = stepFrame[nIter].getColor(
							displace.x + x,
							displace.y + y
							);
						ofColor sliceColor = c[nIter].crop[h].getColor(x, y);

						float noise = ease(
							ofNoise(
								(displace.x + x) / noiseScale,
								(displace.y + y) / noiseScale,
								nIter + (h / static_cast <float>(ITERATIONS))
								),
							noiseScale
							);
						ofColor between = sampleColor.lerp(sliceColor, noise);


						//alphaNoise.setColor(x, y, ofColor(pixColor.r, pixColor.g, pixColor.b, floor(noise * 255.0f)));
						mixedFrame.setColor(x, y, between);
					}
				}
				mixedFrame.pasteInto(stepFrame[nIter].getPixels(), displace.x, displace.y);
				//c[nIter].crop[h].pasteInto(stepFrame[nIter].getPixels(), displace.x, displace.y);

			}else{
				if(nIter > 0){
					stepFrame[nIter - 1].getPixels().cropTo(
						c[nIter].crop[h],
						displace.x,
						displace.y,
						c[nIter].rect[h].getWidth(),
						c[nIter].rect[h].getHeight()
						);
				}else{
					original.getPixels().cropTo(
						c[nIter].crop[h],
						displace.x,
						displace.y,
						c[nIter].rect[h].getWidth(),
						c[nIter].rect[h].getHeight()
						);
				}


				c[nIter].crop[h].pasteInto(stepFrame[nIter].getPixels(), displace.x, displace.y);

			}
			stepFrame[nIter].update();
		}
		if(playingForward){
			c[nIter].distance[h]++;
		}else{
			c[nIter].distance[h]--;
		}
	}
	if(ofGetFrameNum() % 16 == 0){
		playSound("short");
	}

	capture.begin();
	ofClear(0, 255);
	nextFrame();
	capture.end();

	if(isRecording){
		recorder.save(capture.getTexture());
	}

	if(ofGetFrameNum() > 0 && (ofGetFrameNum() % FRAME_PER_ITERATION) == 0){

		if(playingForward){
			if(nIter < ITERATIONS){
				nIter++;
				stepFrame[nIter] = stepFrame[nIter - 1];
				nextMove();
				// resetBillboard();
			}else{
				playingForward = false;
				// resetBillboard();

				// nIter--;
				// playSound("long");
			}
		}else{
			if(nIter > 0){
				nIter--;
				// resetBillboard();
			}else{
				std::cout << "Shutdown" << endl;
				isRecording = false;
				ofExit();
				return;
			}
		}
		std::cout << ofToString(nIter) << "/" << ofToString(ITERATIONS) << " move " << (playingForward ? "forward" : "backward") << endl;
	}
}
//--------------------------------------------------------------

void ofApp::nextFrame(){
	float animT = 1.0f - (ofGetFrameNum() % FRAME_PER_ITERATION) / static_cast <float>(FRAME_PER_ITERATION);
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	stepFrame[nIter].getTexture().bind();
	screenShader.begin();
	screenShader.setUniform1f("u_time", animT / 100.0f);
	screenShader.setUniform2f("u_screen_res", screenWidth, screenHeight);
	screen.draw();
	screenShader.end();
	stepFrame[nIter].getTexture().unbind();
/*
	billboardShader.begin();
	billboardShader.setUniform1f("u_size", ease(animT));
	billboards.draw();
	billboardShader.end();
 */
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
	ofSoundShutdown();

}
