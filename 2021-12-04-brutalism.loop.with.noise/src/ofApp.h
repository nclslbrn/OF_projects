#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"
#include "ofxDirList.h"

#define NUM_BILLBOARDS 5000
#define ITERATIONS 8
#define MOVE_PER_ITERATION 36
#define FRAME_PER_ITERATION 80 // <- Don't change this

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void nextMove();
		void nextFrame();
		void resetBillboard();
		void moveBillboard();
		void loadSound();
		void playSound(string soundType);
		void draw();
		void keyPressed(int key);
		void exit();
		float ease(float p, int g);


		struct Change {
			bool * isVertical = new bool[MOVE_PER_ITERATION];
			bool * goForward = new bool[MOVE_PER_ITERATION];
			ofRectangle * rect = new ofRectangle[MOVE_PER_ITERATION];
			int * distance = new int[MOVE_PER_ITERATION];
			int * size = new int[MOVE_PER_ITERATION];
			int * stepSize = new int[MOVE_PER_ITERATION];
			ofPixels * crop = new ofPixels[MOVE_PER_ITERATION];
		};
		Change * c = new Change[ITERATIONS + 1];
		ofImage * stepFrame = new ofImage[ITERATIONS + 1];

		bool playingForward = true,
			 showInfo = false,
			 isRecording = false;

		float noiseScale = 5000.0f,
			  spreadSize = 12.0f;

		int t = 0,
			screenWidth = 1080,
			screenHeight = 1080,
			shortSndNum,
			longSndNum,
			currShortSound,
			currLongSound;

		ofImage original;
		ofSoundPlayer * shortSound;
		ofSoundPlayer * longSound;
		ofVec2f center;
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

		string imageSource = "scarbor-siu-UYG5IUPoWIo-unsplash.jpg";


		// "sergio-rola-viNlSqFX09k-unsplash.jpg";
		// "chapman-chow-FntjQwDYtXs-unsplash.jpg";
		// "scarbor-siu-UYG5IUPoWIo-unsplash.jpg";
		// "johannes-mandle-JttyHMcXAns-unsplash.jpg";
		// "david-gabric-rL3wztxbxTo-unsplash.jpg";

		// "Dmitri-Popov-on-flickr.com-Herlev-Hospital.jpg";
		// "sergio-rola-viNlSqFX09k-unsplash.jpg";

};