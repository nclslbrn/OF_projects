#pragma once
#include "ofMain.h"
#include "ofxShaderFilter.h"

class FrameMesh {
   public:
    FrameMesh();
    FrameMesh(string imgPath, int threshold, float scale, ofVec2f texcoord);
    void update();
    void drawPoints();
    void drawWireframe();
    void drawFaces();

    ofTexture getTexture();
    int getParticleNum();
    int getWidth();
    int getHeight();

    ofImage img;

    struct Particle {
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec3 color;
    };

    vector<Particle> particles;

    ofVboMesh mesh;
    ofBufferObject buffer;
    ofTexture tex;
    vector<glm::mat4> matrices;
};