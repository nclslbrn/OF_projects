#pragma once

#include "ofMain.h"
#include "ofxDeferredShading.h"
#include "ofxGui.h"
#include "ofxWFC3D.h"

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void setupWPCBuild();
    void drawWPCBuild();
    void keyPressed(int key);

   private:
    void setupDeferred();

    ofEasyCam cam;
    ofLight light;
    ofBoxPrimitive ground;

    // models
    ofNode world_node;
    ofMesh m_line, m_end_l, m_end_r, m_turn, m_base_line, m_base_end_l, m_base_end_r;
    std::unordered_map<std::string, ofMesh*> tiles;
    std::vector<std::pair<std::string, ofNode> > nodes;
    float vs = 2.0;  // model size (2m)
    ofxWFC3D wfc;

    ofxDeferredProcessing deferred;
    ofxDeferred::PointLightPass::Ptr pointLightPass;
    ofxDeferred::ShadowLightPass::Ptr shadowLightPass;
    ofxDeferred::DofPass::Ptr dof;
    ofxDeferred::BloomPass::Ptr bloom;
    ofxDeferred::SsaoPass::Ptr ssao;

    ofxPanel panel;
    bool isShowPanel;
    ofParameter<bool> isShowTile;
    ofParameter<bool> isShowGbuffer;
    ofParameter<bool> isShowShadowMap;
    ofParameter<bool> isShowDofBuffer;
    ofParameter<bool> isShowBloomBuffer;
};
