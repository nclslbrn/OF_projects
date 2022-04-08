#pragma once

#include "ofMain.h"
#include "ofxGifEncoder.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void onGifSaved(string & fileName);
		float noiseScale = 0.1;
		int animFrame = 60, currFrame = 0, noiseLenght = 4;
		ofFbo capture;
		ofImage img;
		ofxGifEncoder gifEncoder;
		bool willRecord, isRecording, isSaved, isOptimizing, isExported = false;

};
