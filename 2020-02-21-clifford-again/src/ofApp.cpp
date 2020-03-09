#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::reset() {
    A = ofRandom(4) - 2;
    B = ofRandom(4) - 2;
    C = ofRandom(4) - 2;
    D = ofRandom(4) - 2;
    i = 0;

    imgName = "A_"+ofToString(A)+
              "_B_"+ofToString(B)+
              "_C_"+ ofToString(C)+
              "_D_"+ofToString(A)+
              ".jpg";

    points.clear();
    std::cout << "A: " << A << " B:" << B << " C:" << C << " D:" << D << endl;
    fbo.clear();
    ofClear(ofColor::black);
}
//--------------------------------------------------------------

//--------------------------------------------------------------
ofColor ofApp::createHue(int n) {
    float hue = ofMap(n, 0, iterations, 0, 255);
    int brightness = 230;
    int saturation = 200;

    return ofColor::fromHsb(hue, saturation, brightness);
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    iterations = 500000;
    width = 3840;
    height = 3840;
    steps = 1000;
    maxSize = 12;
    minSize = 0.25;
    // Generate new constants for attracors
    reset();
    // Set start position
    position.set(0, 0);
    // Set scale for polar coordinate
    minX = -4.0;
    minY = minX * height / width;
    maxX = 4.0;
    maxY = maxX * height / width;
}
//--------------------------------------------------------------
void ofApp::update() {
    if( i < iterations ) {
        for( int j = 0; j < steps; j++ ) {
            float xn = glm::sin(A * position.y) + C * glm::cos(A * position.x);
            float yn = glm::sin(B * position.x) + D * glm::cos(B * position.y);

            float xi = (position.x - minX) * width / (maxX - minX);
            float yi = (position.y - minY) * height / (maxY - minY);

            // skip the first ten points
            if (i > 10) {
                points.push_front(
                    Point(
                        ofVec2f(xi, yi), 
                        createHue(i),
                        1
                    )
                );
            }
            // save for the next iteration
            position.x = xn;
            position.y = yn;
        }
        i+=steps;

        fbo.allocate(width, height, GL_RGBA32F);
        fbo.begin();
        ofBackground(ofColor::black);
        for( auto point : points) {
            point.draw();
        }
        fbo.end();
    
    } 

    if (i == iterations) {
        //if( f == frames ) {
            saveLargeImage(imgName);
            std::cout << "Image generated" << endl;
            reset();
        //} else {
        //    f++;
        //}
    }
   

}

//--------------------------------------------------------------
void ofApp::draw() {
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::saveLargeImage(string imgName) {
	unsigned char* pixels = new unsigned char[3840*3840*3];  ;  
	ofImage saveImage;  
	saveImage.allocate(3840,3840,OF_IMAGE_COLOR);  
	saveImage.setUseTexture(false);  

	fbo.begin();
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, fbo.getWidth(), fbo.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);    
	saveImage.setFromPixels(pixels, fbo.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR);    
	saveImage.saveImage(imgName, OF_IMAGE_QUALITY_MEDIUM);  
	fbo.end();  
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 32) {
        reset();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}
