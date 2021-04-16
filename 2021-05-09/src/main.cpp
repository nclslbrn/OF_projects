#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(960, 720);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
