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
		void nextFrame();
		void draw();
		void keyPressed(int key);
		void exit();

		float ease(float p, int g);
		bool * goForward,
			 * isVertical;

		bool showInfo,
			 isRecording;

		int * size,
			* stepSize,
			* d;

		int halfLoopNum,
			movePerLoop,
			screenWidth,
			screenHeight,
			shortSndNum,
			longSndNum,
			currShortSound,
			currLongSound;

		ofImage original;
		ofImage copy;
		ofRectangle * rect;
		ofSoundPlayer * shortSound;
		ofSoundPlayer * longSound;
		ofPixels * crop;

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

		string imageSource = "johannes-mandle-JttyHMcXAns-unsplash.jpg";
		//"Dmitri-Popov-on-flickr.com-Herlev-Hospital.jpg";

};