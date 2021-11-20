#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"
#include "ofxDirList.h"

#define NUM_BILLBOARDS 5000


class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void nextMove();
		void nextFrame(bool isOverFrame);
		void draw();
		void keyPressed(int key);
		void exit();

		float ease(float p, int g);

		bool goForward,
			 isVertical,
			 showInfo,
			 isRecording;

		int loop,
			loopNum,
			d,
			frameNum,
			size,
			stepSize,
			screenWidth,
			screenHeight,
			imgNum,
			shortSndNum,
			longSndNum,
			currImg,
			currShortSound,
			currLongSound;

		ofImage * image;
		ofRectangle rect;
		ofSoundPlayer * shortSound;
		ofSoundPlayer * longSound;

		ofVec2f center;
		ofPixels crop;
		ofxShader screenShader;
		ofxShader billboardShader;

		ofPlanePrimitive screen;

		// VBO particle of/exmaples/billboardExemple
		float billboardSizeTarget[NUM_BILLBOARDS];
		ofVboMesh billboards;
		glm::vec3 billboardVels[NUM_BILLBOARDS];

		// Screen recording https://forum.openframeworks.cc/t/screenrecording/7090/9
		ofxTextureRecorder recorder;
		ofFbo capture;

		// Assets of/addons/ofxDirList/example
		ofxDirList shortAudioDir;
		ofxDirList longAudioDir;

		std::string imageSource[8] = {
			"annie-spratt-MdijqynYQg8-unsplash.jpg",
			"carl-nenzen-loven-mbfile7XE44-unsplash.jpg",
			"chris-quintero-1vRSJP2V9Nw-unsplash.jpg",
			"daniel-abadia-eQUEEHYmLCE-unsplash.jpg",
			"johannes-mandle-JttyHMcXAns-unsplash.jpg",
			"pete-wright-mT9d7GFl1Zs-unsplash.jpg",
			"scarbor-siu-UYG5IUPoWIo-unsplash.jpg",
			"timelab-pro-xfDnqMh__UQ-unsplash.jpg"
		};


};