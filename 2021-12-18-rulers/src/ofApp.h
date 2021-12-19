#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		float ease(float p, int g);
		ofVec3f getRandomSize();
		ofVec3f getRandomPosition();

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


		struct Ruler {
			ofVec3f position;
			ofVec3f size;
			bool isVertical;
		};

		ofColor * colors = new ofColor[numRuler + 1];

		int numFrame = 60;
		int numRuler = 56;
		float step = 0.7f;
		float domain = 10.0f;
		ofVec2f size = ofVec2f(0.3f, 7.0f);

		Ruler * before = new Ruler[numRuler + 1];
		Ruler * current = new Ruler[numRuler + 1];
		Ruler * after = new Ruler[numRuler + 1];

		ofBoxPrimitive cubeDomain;
		ofEasyCam cam;
};
