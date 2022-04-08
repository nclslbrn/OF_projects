#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

#define NUM_POINTS 5000

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void onGifSaved(string & fileName);
		void exit();
		void frame(float t);
		void keyPressed(int key);
		float ease(float p);
		float softPlus(float q, float p);

		ofVec2f circle(float n);
		ofVec2f astroid(ofVec2f v);
		ofVec2f quadrifolium(ofVec2f v);
		ofVec2f rect_hyperbola(ofVec2f v);
		ofVec2f trifolium(ofVec2f v);
		ofVec2f cardioid(ofVec2f v);
		ofVec2f deltoid(ofVec2f v);
		ofVec2f ranunculoid(ofVec2f v);
		ofVec2f cycloid(ofVec2f v);

		ofVec2f move(ofVec2f pos, float t);
		ofVec2f init();

		float frequency = 10.0f,
			  turbulence = 10.0f,
			  radius = 400.0f,
			  sample = 1.0f;

		int trails = 10,
			margin = 60;

		vector <ofColor> palette = {
			ofColor(247, 222, 232, 5),
			ofColor(253, 239, 217, 5),
			ofColor(215, 254, 239, 5),
			ofColor(218, 231, 251, 5),
			ofColor(26, 34, 45, 5),
		};

		struct Point {
			ofVec2f pos;
			ofColor col;
		};
		Point * points = new Point[NUM_POINTS];

		ofFbo screen;
		ofxGifEncoder gifEncoder;
		int numFrames{60};
		int currFrame = 0;
		ofImage img;
		bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
