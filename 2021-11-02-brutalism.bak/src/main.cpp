#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main(){
	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1920, 1080);
	ofCreateWindow(settings);
	//  ofSetupOpenGL(1080, 1080, OF_WINDOW);
	ofRunApp(new ofApp());
}