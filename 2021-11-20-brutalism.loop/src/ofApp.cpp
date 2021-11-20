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
	bool alreadyProcessedImage = false;
	halfLoopNum = 60;
	screenWidth = 1080;
	screenHeight = 1080;
	movePerLoop = 126;
	isRecording = true;
	showInfo = false;
	ofSetFrameRate(25);
	ofSetWindowShape(screenWidth, screenHeight);
	center = ofVec2f(screenWidth / 2, screenHeight / 2);
	shortSndNum = shortAudioDir.listDir("audio-sample/short");
	longSndNum = longAudioDir.listDir("audio-sample/long");
	shortSound = new ofSoundPlayer[shortSndNum];
	longSound = new ofSoundPlayer[longSndNum];
	string imageFile = (alreadyProcessedImage ? "processed-image/" : "images/1080x1080/") + imageSource;
	original.load(imageFile);
	copy = original;
	//std::cout << "Long audio " + ofToString(longSndNum) << endl;
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
	settings.w = screenWidth;
	settings.h = screenHeight;
	settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
	//settings.pixelFormat = OF_PIXELS_RGB;
	settings.numThreads = 16;
	settings.maxMemoryUsage = 9000000000;
	settings.folderPath = "capture/";
	recorder.setup(settings);

	currLongSound = (int)ofRandom(0, longSndNum);
	if(longSound[currLongSound].isLoaded()){
		std::cout << "Playing long audio sample " << ofToString(currLongSound) << "/" << ofToString(longSndNum - 1) << endl;
		longSound[currLongSound].play();
	}
	// how many frame per loop
	// frameNum = 6 * ceil(ofRandomuf() * 16);
	nextMove();

}

//--------------------------------------------------------------
void ofApp::nextMove(){
	// Graphics
	rect = new ofRectangle[movePerLoop];
	isVertical = new bool[movePerLoop];
	goForward = new bool[movePerLoop];
	d = new int[movePerLoop];
	size = new int[movePerLoop];
	stepSize = new int[movePerLoop];
	crop = new ofPixels[movePerLoop];

	for(int h = 0; h < movePerLoop; h++){
		isVertical[h] = ofRandomuf() > 0.5;
		goForward[h] = ofRandomuf() > 0.5;
		size[h] = ceil((ofRandomuf() * (isVertical[h] ? screenWidth : screenHeight)) * 0.02f);
		stepSize[h] = ceil((ofRandomuf() * (isVertical[h] ? screenHeight : screenWidth)) * 0.3f);
		d[h] = 0;
		float sampleWidth = isVertical[h] ? size[h] : stepSize[h];
		float sampleHeight = isVertical[h] ? stepSize[h] : size[h];
		rect[h].setX(ofRandom(0, screenWidth - sampleWidth));
		rect[h].setY(ofRandom(0, screenHeight - sampleHeight));
		rect[h].setWidth(sampleWidth);
		rect[h].setHeight(sampleHeight);

		copy.getPixels().cropTo(
			crop[h],
			rect[h].getX(),
			rect[h].getY(),
			rect[h].getWidth(),
			rect[h].getHeight()
			);


		// Let sound finnish to play to its end
		if(shortSound[currShortSound].isPlaying()){
			shortSound[currShortSound].stop();
		}


		currShortSound = (int)ofRandom(0, shortSndNum);
		if(shortSound[currShortSound].isLoaded()){
			shortSound[currShortSound].play();
		}
	}
	for(int i = 0; i < NUM_BILLBOARDS; i++){
		int randRect = (int)ofRandom(0, movePerLoop);
		size_t x = rect[randRect].getX() + (ofRandom(0, rect[randRect].getWidth()));
		size_t y = rect[randRect].getY() + (ofRandom(0, rect[randRect].getHeight()));

		billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
		billboards.getVertices()[i] = {x, y, 0};
		billboards.getColors()[i].set(
			copy.getColor(x, y));
		billboardSizeTarget[i] = 8 * floor(ofRandom(8));
		// stepSize[randRect] * ofRandomuf();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSoundUpdate();

	if(shortSound[currShortSound].isLoaded() && !shortSound[currShortSound].isPlaying()){
		shortSound[currShortSound].play();
	}

	for(int h = 0; h < movePerLoop; h++){
		ofVec2f displace(
			rect[h].getX() + (goForward[h] ? 1 : -1) * (isVertical[h] ? d[h] : 0) * rect[h].getWidth(),
			rect[h].getY() + (goForward[h] ? 1 : -1) * (isVertical[h] ? 0 : d[h]) * rect[h].getHeight()
			);

		if(displace.x > 0 && displace.y > 0 && displace.x < screenWidth && displace.y < screenHeight){
			if(ofGetFrameNum() <= halfLoopNum){

				crop[h].pasteInto(copy.getPixels(), displace.x, displace.y);
			}else{
				original.getPixels().cropTo(
					crop[h],
					displace.x,
					displace.y,
					rect[h].getWidth(),
					rect[h].getHeight()
					);
				crop[h].pasteInto(copy.getPixels(), displace.x, displace.y);
			}
			copy.update();
		}
		if(ofGetFrameNum() <= halfLoopNum){
			d[h]++;
		}else{
			d[h]--;
		}
	}


	float t =
		1.0f - ease((ofGetFrameNum() % halfLoopNum) / static_cast <float>(halfLoopNum), 5);

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
	if(ofGetFrameNum() > 0 && ofGetFrameNum()  == halfLoopNum * 2){
		isRecording = false;
		ofExit();
	}
}
//--------------------------------------------------------------

void ofApp::nextFrame(){
	float animT = 1.0f - ((ofGetFrameNum() % halfLoopNum) / (float)halfLoopNum);
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
	copy.save("processed-image/" + imageSource);


}
