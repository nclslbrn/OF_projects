#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

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


		float sample = 2.5f,
			  density = 0.03f,
			  radius = 300.0f,
			  phi = (sqrt(5) + 1) / 2;

		int trails = 5,
			margin = 10;

		vector <ofColor> palette = {
			ofColor(247, 222, 232),
			ofColor(253, 239, 217),
			ofColor(215, 254, 239),
			ofColor(218, 231, 251),
			ofColor(26, 34, 45),
		};

		struct Point {
			ofVec2f pos;
			ofColor col;
		};
		vector <Point> points;

		ofFbo screen;
		ofEasyCam cam;
		ofxGifEncoder gifEncoder;
		int numFrames{60};
		int currFrame = 0;
		ofImage img;
		bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;
};
