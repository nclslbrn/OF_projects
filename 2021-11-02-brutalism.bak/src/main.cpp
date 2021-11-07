#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
  ofGLWindowSettings settings;
  settings.setGLVersion(3, 2);
  // WQXGA 	2 560 × 1 600
  settings.setSize(1080, 1080);
  ofCreateWindow(settings);
  //  ofSetupOpenGL(1080, 1080, OF_WINDOW);
  ofRunApp(new ofApp());
}