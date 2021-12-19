#pragma once
#include "FrameMesh.h"
#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"


class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);

		ofVec2f center;

		int screenWidth = 2880,
			screenHeight = 1620,
			framesInLoop = 6,
			minPixelsBrightness = 65;

		float meshScale = 12.0;
		ofVideoPlayer video;
		vector <FrameMesh> frames;
		ofxShader shader;
		ofEasyCam camera;
		ofImage sparkTexture;
		ofVec2f sparkTexCoord;

		bool isRecording = true;
		int recordFrameNum = 0;

		// Screen recording https://forum.openframeworks.cc/t/screenrecording/7090/9
		ofxTextureRecorder recorder;
		ofFbo capture;
};
