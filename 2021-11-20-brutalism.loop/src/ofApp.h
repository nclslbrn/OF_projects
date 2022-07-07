#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"
#include "ofxDirList.h"

// 250 frame

#define NUM_BILLBOARDS 5000
#define ITERATIONS 4
#define MOVE_PER_ITERATION 50
#define FRAME_PER_ITERATION 25

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
			 isRecording = true;

		int nIter = 0,
			screenWidth = 1080,
			screenHeight = 1920,
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
		// 1080x1080
		// "chapman-chow-FntjQwDYtXs-unsplash.jpg"
		// "sergio-rola-gA-vxrqmYV8-unsplash.jpg"
		// "mark-hang-fung-so-n9XVNuR_TQU-unsplash.jpg";
		// "skull-kat-nMHY0SHuiVQ-unsplash.jpg"
		// "sergio-rola-viNlSqFX09k-unsplash.jpg"
		// "serge-george-fRCA008qXPs-unsplash.jpg"
		// "chuttersnap-7HBiUfgwBzo-unsplash.jpg"
		// "chuttersnap-DoetkvzRrss-unsplash.jpg"
		// "raphael-koh-VJ9JYFkZv5A-unsplash.jpg";
		// "thanh-le-7UdFnHMu7q8-unsplash.jpg";
		// "usamah-khan-fFy__IX0lpM-unsplash.jpg"
		string imageSource = "alex-rodriguez-santibanez-U0PCvzknU7c-unsplash.jpg";
		// "chapman-chow-FntjQwDYtXs-unsplash.jpg";
		// "sergio-rola-viNlSqFX09k-unsplash.jpg";
		// "david-gabric-rL3wztxbxTo-unsplash.jpg";
		// "scarbor-siu-UYG5IUPoWIo-unsplash.jpg";
		// "johannes-mandle-JttyHMcXAns-unsplash.jpg";

		//
		// "david-gabric-rL3wztxbxTo-unsplash.jpg";

		// "johannes-mandle-JttyHMcXAns-unsplash.jpg";
		// "Dmitri-Popov-on-flickr.com-Herlev-Hospital.jpg";
		// "sergio-rola-viNlSqFX09k-unsplash.jpg";

};