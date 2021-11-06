#include "ofApp.h"
//#include "ofConstants.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);
    sample.load("images/sergio-rola-viNlSqFX09k-unsplash.jpg");
    //sample.load("images/ivan-aleksic-FoYLV60_eHY-unsplash.jpg");
    screenShader.load("shaders/Screen");
    billboardShader.load("shaders/Billboard");

    screen.set(ofGetWidth(), ofGetHeight());
    screen.mapTexCoords(0, 0, sample.getWidth(), sample.getHeight());

    // of/exmaples/billboardExemple
    billboards.getVertices().resize(NUM_BILLBOARDS);
    billboards.getColors().resize(NUM_BILLBOARDS);
    billboards.getNormals().resize(NUM_BILLBOARDS, glm::vec3(0));

    // ------------------------- billboard particles

    billboards.setUsage(GL_DYNAMIC_DRAW);
    billboards.setMode(OF_PRIMITIVE_POINTS);

    nextMove();
}
//--------------------------------------------------------------
void ofApp::nextMove() {
    size = 1.0f + (ofRandomuf() * (float)ofGetWidth() * 0.01f);
    stepSize = 1.0f + ofRandomuf() * 120.0f;
    numFrame = (int)(10 * glm::ceil(ofRandomuf() * 10));
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
    sampleCroped.crop(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
    sample.getPixels()
        .cropTo(crop, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());

    for (int i = 0; i < NUM_BILLBOARDS; i++) {
        ofVec2f particlePos( 
            rect.getX() + (ofRandomf() * rect.getWidth() * 0.5), 
            rect.getY() + (ofRandomf() * rect.getHeight() * 0.5)
        );

        billboardVels[i] = {ofRandomf(), -1.0, ofRandomf()};
        billboards.getVertices()[i] = {
            particlePos.x,
            particlePos.y,
            0
        };

        billboards.getColors()[i].set(
            sample.getColor(particlePos.x, particlePos.y)
            //ofColor::fromHsb(ofRandom(0, 96), 255, 255)
        );
        billboardSizeTarget[i] = stepSize * ofRandomuf();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    int f = (int)ofGetFrameNum();

    if (f % numFrame == 0) {
        nextMove();
    } else {
        ofVec2f displace(
            rect.getX() + (goForward ? 1 : -1) * (isVertical ? d : 0) * rect.getWidth(),
            rect.getY() + (goForward ? 1 : -1) * (isVertical ? 0 : d) * rect.getHeight());

        
        if (displace.x < 0) {
            displace.x += sample.getWidth();
        }
        if (displace.y < 0) {
            displace.y += sample.getHeight();
        }

        crop.pasteInto(sample.getPixels(), displace.x, displace.y);
        sample.update();
        d++;

        float t = 1.0f -((f % numFrame) / static_cast<float>(numFrame));
        //float div = 250.0;

        for (int i = 0; i < NUM_BILLBOARDS; i++) {
            // noise
           /*  glm::vec3 vec(ofSignedNoise(t, billboards.getVertex(i).y / div, billboards.getVertex(i).z / div),
                          ofSignedNoise(billboards.getVertex(i).x / div, t, billboards.getVertex(i).z / div),
                          ofSignedNoise(billboards.getVertex(i).x / div, billboards.getVertex(i).y / div, t));

 */            
            glm::vec3 vec(0, t + ofSignedNoise(billboards.getVertex(i).x, billboards.getVertex(i).y, 0, 0), 0);
            billboardVels[i] += vec;
            billboardVels[i].y += 0.25f;
            billboards.getVertices()[i] += billboardVels[i];
            billboardVels[i] *= 0.94f;
            billboards.setNormal(i, glm::vec3(12 + billboardSizeTarget[i] * ofNoise(t + i), 0, 0));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    // ofBackgroundGradient(ofColor(255), ofColor(230, 240, 255));
    float animT = 1.0f - ((ofGetFrameNum() % numFrame)/(float)numFrame);
    cam.begin();

    sample.getTexture().bind();
    screenShader.begin();
    screenShader.setUniform2f("u_screen_res", ofGetWidth(), ofGetHeight());
    screen.draw();
    screenShader.end();
    sample.getTexture().unbind();
    
    cam.end();
    
    ofEnableAlphaBlending();
    ofPushMatrix();
    sampleCroped.getTexture().bind();
    ofEnablePointSprites();
    billboardShader.begin();
    billboardShader.setUniform1f("u_alpha", animT);
    billboardShader.setUniform2f("u_crop_res", sampleCroped.getWidth(), sampleCroped.getHeight());
    billboardShader.setUniformTexture("u_crop_tex", sampleCroped.getTexture(), 0);
    billboards.draw();
    ofDisablePointSprites();
    billboardShader.end();
    sampleCroped.getTexture().unbind();
    ofPopMatrix();
    ofDisableAlphaBlending();


    string info = ofToString(ofGetFrameRate(), 2) + "\n";
    string time = ofToString(animT, 2) + "\n";
    info += time;
    info += "Particle Count: " + ofToString(NUM_BILLBOARDS);
    ofDrawBitmapStringHighlight(info, 30, 30);
    ofSetColor(255);
    sampleCroped.draw(4, 4);
}