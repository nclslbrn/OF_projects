#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"

#define NUM_BILLBOARDS 10000

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void nextMove();
		void nextFrame();
		void draw();
		void keyPressed(int key);

		bool goForward;
		bool isVertical;
		ofRectangle rect;
		float stepSize;
		float size;
		int d;
		int screenWidth, screenHeight;
		int numFrame;

		string imageSource[5] = {
			"annie-spratt-MdijqynYQg8-unsplash.jpg",
			"carl-nenzen-loven-mbfile7XE44-unsplash.jpg",
			"moollyem-MXkVCT9hbs4-unsplash.jpg",
			"pete-wright-mT9d7GFl1Zs-unsplash.jpg",
			"timelab-pro-xfDnqMh__UQ-unsplash.jpg"
		};

		ofVec2f center;
		ofImage sample;
		ofImage sampleCroped;
		ofPixels crop;
		ofxShader screenShader;
		ofxShader billboardShader;

		ofPlanePrimitive screen;

		// VBO particle of/exmaples/billboardExemple
		float billboardSizeTarget[NUM_BILLBOARDS];
		ofVboMesh billboards;
		glm::vec3 billboardVels[NUM_BILLBOARDS];

		// Screen recording https://forum.openframeworks.cc/t/screenrecording/7090/9
		bool isRecording;
		ofxTextureRecorder recorder;
		ofFbo capture;

		bool showInfo;
};