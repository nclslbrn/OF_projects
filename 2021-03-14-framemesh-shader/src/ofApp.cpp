#include "ofApp.h"

#include "ofConstants.h"
#include "ofGstVideoPlayer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	// this prevent frame drop
	ofSetFrameRate(25);
	ofDisableArbTex();
	ofEnablePointSprites();
	ofEnableAlphaBlending();

	center = ofVec2f(screenWidth / 2.0f, screenHeight / 2.0f);

	// initialize video player
	video.setLoopState(OF_LOOP_NORMAL);
	video.setPlayer(ofPtr <ofGstVideoPlayer>(new ofGstVideoPlayer));
	video.loadMovie("videos/stalker.frontier.avi");
	video.setUseTexture(false);
	video.play();
	// we will pass frame manually so put video on pause
	video.setPaused(true);
	shader.load("shaders/particle");

	if(video.isLoaded()){
		frames.resize(framesInLoop - 1);
		// load spark texture
		sparkTexture.allocate(248, 248, OF_IMAGE_COLOR_ALPHA);
		sparkTexture.load("textures/spark-2.png");
		sparkTexCoord = ofVec2f(
			sparkTexture.getWidth(),
			sparkTexture.getHeight());
		video.nextFrame();
		video.update();

		for(int i = 1; i < framesInLoop - 1; i++){
			if(video.isFrameNew()){
				ofPixels framePixels = video.getPixels();
				FrameMesh j = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
				j.compute(shader);
				j.unvalidate();
				frames.push_back(j);
				video.nextFrame();
				video.update();
			}
		}

		shader.begin();
		shader.setUniform2f("u_screenRes", ofGetWidth(), ofGetHeight());

		shader.setUniform1f("u_scale", meshScale);
		shader.end();

		camera.setVFlip(true);
		camera.setDistance(meshScale * screenWidth * 1.75);
		camera.setNearClip(0.001);
		camera.setFarClip(-meshScale * screenWidth * 2.0);
		camera.setFov(15.0f);
		// camera.setPosition(0, 0, 60000);
		// camera.setTarget(glm::vec3(0, 0, 0));

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
	}else{
		std::cout << "Video not loaded." << endl;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
	// move frame forward

	if(video.isFrameNew()){
		for(int i = framesInLoop - 1; i > 0; i--){
			frames[i] = frames[i - 1];
		}
		// record on only if new frame append to prevent
		// recording when the apps is laggin
		if(isRecording && ofGetFrameNum() > 0){
			recorder.save(capture.getTexture());
			recordFrameNum++;
		}

		// replace a mesh on the scene with a new one
		ofPixels framePixels = video.getPixels();
		FrameMesh newFrame = FrameMesh(framePixels, minPixelsBrightness, meshScale, sparkTexCoord);
		newFrame.compute(shader);
		frames[0] = newFrame;
		video.nextFrame();
	}else{
		// give it a new try
		// frames[0].unvalidate();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableAlphaBlending();
	ofEnablePointSprites();

	ofEnableDepthTest();
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	if(isRecording){
		capture.begin();
	}
	ofClear(0);
	ofBackground(0);
	//ofBackgroundGradient(ofColor(0), ofColor(40), OF_GRADIENT_CIRCULAR);
	camera.begin();
	shader.begin();
	shader.bindDefaults();

	for(int i = 0; i < frames.size(); i++){
		ofPushMatrix();
		ofTranslate(0, -120, i * 3000.0f);

		if(frames[i].isTexAllocated() && frames[i].isValidFrame()){
			shader.setUniform1f("u_time", ofGetFrameNum() * 0.0001f);
			shader.setUniform1f("u_layer", i / static_cast <float>(framesInLoop));
			sparkTexture.getTexture().bind();
			shader.setUniformTexture("spark", sparkTexture.getTexture(), 2);
			shader.setUniformTexture("u_frameTex", frames[i].getTexture(), 0);

			frames[i].drawPoints();
			sparkTexture.getTexture().unbind();
		}
		ofPopMatrix();
	}

	shader.end();
	camera.end();
	if(isRecording){
		capture.end();
		capture.draw(0, 0, 1920, 1080);
	}

	ofDisableDepthTest();
	ofDisablePointSprites();
	ofDisableAlphaBlending();

	ofDrawBitmapString(
		ofToString(ofGetFrameRate()) + " fps",
		20, 20);
	ofDrawBitmapString(
		ofToString(frames[0].getParticleNum()) + " particles",
		ofGetWidth() - 120, 20);
	if(isRecording){
		ofDrawBitmapString("Recording", 20, ofGetHeight() - 20);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 114 || key == 'r'){
		isRecording = !isRecording;
		std::cout << (isRecording ? "start recording" : "stop recording") << endl;
	}else{
		std::cout << "Unaffected key = " << key << endl;
	}
}

//--------------------------------------------------------------
void ofApp::exit(){
}
