#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"
#include "ofxDirList.h"

#define NUM_BILLBOARDS 5000
#define NUM_MOVE 54 // 54

class ofApp : public ofBaseApp {
	public:
		void loadSound(bool verbose);
		void playSound(string soundType);
		void setup();
		void update();
		void nextMove();
		void move();
		void nextFrame();
		void draw();
		void keyPressed(int key);
		void exit();

		float ease(float p, int g);

		bool showInfo = false,
			 isRecording = false,
			 alreadyProcessedImage = false;

		struct Move {
			bool isVertical;
			bool isStopped;
			bool goForward;
			int distance;
			int size;
			int stepSize;
			float spreadSize;
			float noiseScale;
			ofRectangle rect;
			ofPixels crop;
		};
		Move m[NUM_MOVE + 1];


		int loop = 0,
			loopNum = 16,
			screenWidth = 2880,
			screenHeight = 1620,
			frameNum,
			imgNum,
			shortSndNum,
			longSndNum,
			currImg,
			currShortSound,
			currLongSound;

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

		ofImage * image;
		ofSoundPlayer * shortSound;
		ofSoundPlayer * longSound;

		ofVec2f center = ofVec2f(screenWidth / 2, screenHeight / 2);
		ofPlanePrimitive screen;
		ofxShader screenShader;
		ofxShader billboardShader;



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




};