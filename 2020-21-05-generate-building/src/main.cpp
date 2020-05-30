#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
    /* OpenGL 4.1 for ofxDeferredShading */
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 1);
    settings.setSize(1080, 1080);

    ofCreateWindow(settings);
    /* default openGL ofSetupOpenGL(1024,768,OF_WINDOW); */
    ofRunApp(new ofApp());
}
