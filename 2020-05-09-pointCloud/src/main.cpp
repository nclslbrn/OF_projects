#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
    /* --------- OPENGL RENDERING --------- */
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(1200, 768);
    ofCreateWindow(settings);
    /* --------- DEFAULT RENDERING --------
	ofSetupOpenGL(1024,768,OF_WINDOW);
	*/
    ofRunApp(new ofApp());
}
