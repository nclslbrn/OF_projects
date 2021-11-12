#pragma once

#include "ofMain.h"
#include "ofxShaderFilter.h"
#include "ofxTextureRecorder.h"
#include "ofxDirList.h"

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
		ofxDirList shortAudioDir;
		ofxDirList longAudioDir;

		int nShortAudioSample;
		int nLongAudioSample;
		ofSoundPlayer * shortAudioSample;
		ofSoundPlayer * longAudioSample;

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

		/*
		string shortAudioSource[18] = {
			"123J31.mp3",
			"377058-Low_Drum_Tom-Resonating_Head-Rattling_Bolts-Hitting_With_Drumstick-04.mp3",
			"466192-Bowed_Alien_Metals_09.mp3",
			"blip-808-echo.mp3",
			"button_small_servo_06.mp3",
			"CBS_BEATSOUND_kick_808ish.mp3",
			"CBS_BEATSOUND_kick_808ish2.mp3",
			"CyberSet_001_45.mp3",
			"electro_hit.mp3",
			"Female_video_game_voice_saying_-alert_code_808_disturbing_the_peace-.mp3",
			"FT1_Fmin_Trap_Kit_808_Bass.mp3",
			"MC_01_Snare_01.mp3",
			"metal_goosh_explo.mp3",
			"MiKlo23.mp3",
			"P9OwS.mp3",
			"pause_interface_29.mp3",
			"Percussion_110.mp3",
			"qQq2qQq.mp3",
			"Scrapes-and-Scratches_0808.mp3"
		};
		string longAudioSource[NUM_LONG_AUDIO] = {
			"Drone-Sci-Fi-Abstract-Simple-Ghostly_Fluttering_Sound_Throughout.mp3",
			"Extract_from_CD_15_-_Track_15.mp3",
			"GSisaacD098.mp3",
			"helx0X0x.mp3",
			"PM_FN_Music_SeamlessLoop_1B.mp3",
			"PM_FN_Music_SeamlessLoop_1I.mp3"
		};
		*/
};