#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
    /* --------- Specified opengl version --------- */
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(1200, 750);
    ofCreateWindow(settings);
    /* ---------------------- */
    /* --------- or use the default one -------- 
    ofSetupOpenGL(1024, 768, OF_WINDOW);
     -------------- */

    ofRunApp(new ofApp());
}
