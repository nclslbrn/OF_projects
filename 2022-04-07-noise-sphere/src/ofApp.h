#pragma once

#include "ofMain.h"
#define NUM_POINTS 1000

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();

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
		float ease(float p);
		float softPlus(float q, float p);

		float noiseFrequency = 0.07,
			  noiseStrenght = 200.0f,
			  radius = 200.0f;

		int animFrame = 320;

		vector <ofColor> palette = {
			ofColor(0, 91, 197),
			ofColor(0, 180, 252),
			ofColor(23, 249, 255),
			ofColor(223, 147, 0),
			ofColor(248, 190, 0)};

		struct Point {
			ofVec3f pos;
			ofColor col;
		};
		Point * points = new Point[NUM_POINTS];
};
