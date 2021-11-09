#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  isRecording = false;
  center = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);

  sample.load("images/2880x1620/chuttersnap-otN9QPy_veg-unsplash.jpg");
  screenShader.load("shaders/Screen");
  billboardShader.load("shaders/Billboard");

  screen.set(ofGetWidth(), ofGetHeight());
  screen.setPosition(center.x, center.y, 0);
  screen.setScale(1, -1, 1);
  screen.mapTexCoords(0, 0, sample.getWidth(), sample.getHeight());

  // of/exmaples/billboardExemple
  billboards.getVertices().resize(NUM_BILLBOARDS);
  billboards.getColors().resize(NUM_BILLBOARDS);
  billboards.getNormals().resize(NUM_BILLBOARDS, glm::vec3(0));
  billboards.setUsage(GL_DYNAMIC_DRAW);
  billboards.setMode(OF_PRIMITIVE_POINTS);
  // of/addons/ofxTextureRecorder/example
  capture.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
  ofxTextureRecorder::Settings settings(capture.getTexture());
	settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
	settings.numThreads = 4;
	settings.maxMemoryUsage = 9000000000;
	recorder.setup(settings); 

  nextMove();
}
//--------------------------------------------------------------
void ofApp::nextMove() {
  size = 1.0f + (ofRandomuf() * (float)ofGetWidth() * 0.1f);
  stepSize = 1.0f + ofRandomuf() * 460.0f;
  numFrame = (int)(6 * glm::ceil(ofRandomuf() * 10));
  goForward = ofRandomuf() > 0.5;
  isVertical = ofRandomuf() > 0.5;
  d = 0;
  rect.setX(roundf(ofRandomuf() * sample.getWidth()));
  rect.setY(roundf(ofRandomuf() * sample.getHeight()));
  rect.setWidth(isVertical ? size : stepSize);
  rect.setHeight(isVertical ? stepSize : size);

  if (rect.getX() + rect.getWidth() > ofGetWidth()) {
    rect.setWidth(rect.getWidth() * -1);
  }

  if (rect.getX() + rect.getHeight() > ofGetHeight()) {
    rect.setHeight(rect.getHeight() * -1);
  }

  sampleCroped = sample;
  sampleCroped.crop(rect.getX(), rect.getY(), rect.getWidth(),
                    rect.getHeight());
  sample.getPixels().cropTo(crop, rect.getX(), rect.getY(), rect.getWidth(),
                            rect.getHeight());

  for (int i = 0; i < NUM_BILLBOARDS; i++) {
    ofVec2f particlePos(rect.getX() + (ofRandomf() * rect.getWidth() * 0.5),
                        rect.getY() + (ofRandomf() * rect.getHeight() * 0.5));

    billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
    billboards.getVertices()[i] = {particlePos.x, particlePos.y, 0};
    billboards.getColors()[i].set(
        sample.getColor((int)particlePos.x, (int)particlePos.y));
    billboardSizeTarget[i] = stepSize * ofRandomuf();
  }
}

//--------------------------------------------------------------
void ofApp::update() {

  if ((int)ofGetFrameNum() % numFrame == 0) {
    nextMove();
  } else {
    ofVec2f displace(rect.getX() + (goForward ? 1 : -1) * (isVertical ? d : 0) *
                                       rect.getWidth(),
                     rect.getY() + (goForward ? 1 : -1) * (isVertical ? 0 : d) *
                                       rect.getHeight());

    if (displace.x < 0) {
      displace.x += sample.getWidth();
    }
    if (displace.y < 0) {
      displace.y += sample.getHeight();
    }

    crop.pasteInto(sample.getPixels(), displace.x, displace.y);
    sample.update();
    d++;

    float t =
        1.0f - ((ofGetFrameNum() % numFrame) / static_cast<float>(numFrame));

    for (int i = 0; i < NUM_BILLBOARDS; i++) {

      glm::vec3 vec(0,
                    t + ofSignedNoise(billboards.getVertex(i).x,
                                      billboards.getVertex(i).y, 0, 0),
                    0);
      billboardVels[i] += vec;
      // billboardVels[i].y *= 1.01f;
      billboards.getVertices()[i] += billboardVels[i];
      billboardVels[i] *= 0.94f;
      billboards.setNormal(
          i, glm::vec3(12 + billboardSizeTarget[i] * ofNoise(t + i), 0, 0));
    }


    capture.begin();
    ofClear(255, 255);
    nextFrame();
    capture.end();

    if(ofGetFrameNum()>0){
		  recorder.save(capture.getTexture());
	  }
	  if(ofGetFrameNum() > ofGetWidth() + 50){
		  ofExit(0);
	  } 
  }
}
//--------------------------------------------------------------

void ofApp::nextFrame() {
  float animT = 1.0f - ((ofGetFrameNum() % numFrame) / (float)numFrame);

  ofPushMatrix();
  sample.getTexture().bind();
  screenShader.begin();
  screenShader.setUniform2f("u_screen_res", ofGetWidth(), ofGetHeight());
  screen.draw();
  screenShader.end();
  sample.getTexture().unbind();
  ofPopMatrix();

  ofEnableAlphaBlending();
  sampleCroped.getTexture().bind();
  ofEnablePointSprites();
  billboardShader.begin();
  billboardShader.setUniform1f("u_size", animT);
  billboards.draw();
  ofDisablePointSprites();
  billboardShader.end();
  sampleCroped.getTexture().unbind();
  ofDisableAlphaBlending();

}
//--------------------------------------------------------------
void ofApp::draw() {
  capture.draw(0,0,1920, 1080);

  // capture.draw(0,0);
  // debug & info
  string info = ofToString(ofGetFrameRate(), 2) + "\n";
  info += "Particle Count: " + ofToString(NUM_BILLBOARDS);
  ofDrawBitmapStringHighlight(info, 30, 30);
  ofSetColor(255);
  sampleCroped.draw(4, 4);
}