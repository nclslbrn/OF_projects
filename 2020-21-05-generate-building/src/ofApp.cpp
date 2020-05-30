#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup() {
    // SCENE
    cam.setDistance(10);
    cam.setFarClip(20.0f);
    cam.setNearClip(0.01);
    light.move(2, -5, -1);
    light.lookAt(glm::vec3(0.0, 0.0, 0.0));
    light.setDirectional();
    light.setDiffuseColor(ofColor(240, 240, 210));
    light.setAmbientColor(ofColor(60, 50, 90));
    light.setup();
    ground.set(30, 30, 1);
    ground.rotate(90, 1.0, 0, 0);
    ground.setPosition(0, -0.5, 0);
    setupWPCBuild();
    setupDeferred();
    panel.add(isShowTile.set("show_tile", false));
    panel.add(isShowGbuffer.set("show_gbuffer", false));
    panel.add(isShowShadowMap.set("show_shadowMap", false));
    panel.add(isShowDofBuffer.set("show_dofBuffers", false));
    panel.add(isShowBloomBuffer.set("show_bloomBuffers", false));
}
//--------------------------------------------------------------
void ofApp::setupDeferred() {
    deferred.init(ofGetWidth(), ofGetHeight());

    auto bg = deferred.createPass<ofxDeferred::BgPass>();
    auto e = deferred.createPass<ofxDeferred::EdgePass>();

    ssao = deferred.createPass<ofxDeferred::SsaoPass>();
    shadowLightPass = deferred.createPass<ofxDeferred::ShadowLightPass>();
    pointLightPass = deferred.createPass<ofxDeferred::PointLightPass>();
    pointLightPass->addLight();
    pointLightPass->addLight();
    deferred.createPass<ofxDeferred::FxaaPass>();

    deferred.createPass<ofxDeferred::FogPass>();
    dof = deferred.createPass<ofxDeferred::DofPass>();
    bloom = deferred.createPass<ofxDeferred::BloomPass>();
    /*  ssao->setOcculusionRadius(5.0);
    ssao->setDarkness(0.5); */
    panel.setup();
    panel.setDefaultBackgroundColor(ofFloatColor(0.5, 0.5, 0.5, 0.5));
    panel.setDefaultHeaderBackgroundColor(ofFloatColor(0.6, 0.6, 0.3, 0.6));
    panel.setDefaultFillColor(ofFloatColor(0.0, 0.0, 0.0, 1.0));
    panel.add(deferred.getParameters());
    panel.minimizeAll();
    panel.loadFromFile("settings.xml");
}
//--------------------------------------------------------------
void ofApp::setupWPCBuild() {
    // MODELS
    // model made on blender and exported as ply
    // Y Up
    // Z forward
    m_line.load("plys/line.ply");
    m_end_l.load("plys/end_l.ply");
    m_end_r.load("plys/end_r.ply");
    m_turn.load("plys/turn.ply");
    m_base_line.load("plys/base_line.ply");
    m_base_end_l.load("plys/base_end_l.ply");
    m_base_end_r.load("plys/base_end_r.ply");

    // set tile-name mapping
    tiles["line"] = &m_line;
    tiles["end_l"] = &m_end_l;
    tiles["end_r"] = &m_end_r;
    tiles["turn"] = &m_turn;
    tiles["base_line"] = &m_base_line;
    tiles["base_end_l"] = &m_base_end_l;
    tiles["base_end_r"] = &m_base_end_r;

    world_node.setOrientation(glm::angleAxis(ofDegToRad(0.f), glm::vec3{1.f, 0.f, 0.f}));
}
//--------------------------------------------------------------

void ofApp::drawWPCBuild() {
    auto render = ofGetCurrentRenderer();
    if (isShowTile) ofDrawGrid(1.0, 10, false, false, true, false);
    ground.draw();

    for (auto& node : nodes) {
        auto key = node.first;
        node.second.transformGL();

        tiles[key]->drawFaces();
        node.second.restoreTransformGL();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    shadowLightPass->setGlobalPosition(glm::normalize(glm::vec3(cos(ofGetElapsedTimef() * 0.5), 0.4f + 0.2f * sin(ofGetElapsedTimef() * 0.5f), sin(ofGetElapsedTimef() * 0.5))) * 1600.f);
    shadowLightPass->lookAt(glm::vec3(0));
}
//--------------------------------------------------------------
void ofApp::draw() {
    shadowLightPass->beginShadowMap(cam);
    pointLightPass->drawLights();
    drawWPCBuild();
    shadowLightPass->endShadowMap();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    deferred.begin(cam);
    drawWPCBuild();
    glDisable(GL_CULL_FACE);
    deferred.end();

    if (isShowPanel) {
        if (isShowTile) {
            auto render = ofGetCurrentRenderer();

            for (auto& node : nodes) {
                auto key = node.first;
                //node.second.transformGL();
                // show rotation
                ofSetColor(255);
                int ri = int(node.second.getOrientationEulerRad().y + 2.0);
                if (ri == 0)
                    ri = 3;
                else if (ri == 1)
                    ri = 2;
                else if (ri == 2)
                    ri = 0;
                else if (ri == 3)
                    ri = 1;
                render->drawString(ofToString(ri), 0, 0.5, 0);
                //node.second.restoreTransformGL();
            }
        }
        if (isShowGbuffer) deferred.debugDraw();
        if (isShowShadowMap) shadowLightPass->debugDraw(glm::vec2(ofGetWidth() - 512, 0), glm::vec2(512));
        if (isShowDofBuffer) dof->debugDraw(glm::vec2(0), glm::vec2(ofGetWidth(), ofGetHeight()) * 0.25f);
        if (isShowBloomBuffer) bloom->debugDraw(glm::vec2(0, ofGetHeight() * 0.25), glm::vec2(ofGetWidth(), ofGetHeight()) * 0.25f);
        panel.draw();
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 18, 24);
    }

    /*
    ofDisableDepthTest();
    ofDisableLighting();
    ofDrawBitmapStringHighlight("FPS " + ofToString(ofGetFrameRate(), 0), 20, 20);

    ofDrawBitmapString("PRESS SPACEBAR TO GENERATE A NEW STRUCTURE", 20, ofGetHeight() - 40);
    ofDrawBitmapString("PRESS S TO VISUALIZE THE TILES", 20, ofGetHeight() - 20);
    */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 115) {
        isShowPanel = !isShowPanel;
    } else if (key == 32) {
        int x = 15, y = 5, z = 15;

        // config_name, subset, x, y, z, periodic=false, ground="", surround=false
        wfc.SetUp("data.xml", "default", x, y, z, false, "", "empty");

        // instanciate an specific tile on the WFC
        /*
         *for (int ix = 1; ix < x-1; ix++)
         *    for (int iz = 1; iz < z-1; iz++)
         *        wfc.SetTile("empty", ix,y-1,iz);
         */

        int limit = 8, seed = (int)ofRandom(1000);
        for (int k = 0; k < limit; k++) {
            bool result = wfc.Run(seed++);

            if (result) {
                ofLog() << "WFC success";

                // process tiles and convert to ofNode tree
                nodes = wfc.NodeTileOutput(world_node, glm::vec3(vs, vs, vs), {"empty"});
                break;
            } else {
                ofLog() << "WFC failure";
            }
        }

        world_node.setPosition(-x / 2.0, 0, -z / 2.0);
        world_node.move(0.5, 0.5, 0.5);
        world_node.setScale(1 / vs);
    } else {
        ofLog() << "Unknow key: " << key;
        pointLightPass->addLight();
    }
}
