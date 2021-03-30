#pragma once
#include "ofMain.h"
#include "ofxShaderFilter.h"

class FrameMesh {
   public:
    FrameMesh();
    FrameMesh(ofPixels framePixels, int threshold, float scale, ofVec2f texcoord);
    void compute();
    void update();
    void drawPoints();
    void drawWireframe();
    void drawFaces();

    ofTexture getTexture();
    int getParticleNum();
    int getWidth();
    int getHeight();
    bool isTexAllocated();

    struct Particle {
        glm::vec4 pos;
        glm::vec3 rot;
        glm::vec3 color;
        float size;
    };

    vector<Particle> particles;

    ofPixels pixels;
    int brightThreshold;
    float meshScale;
    ofVec2f coord;
    ofVboMesh mesh;
    ofBufferObject buffer;
    ofTexture tex;
    vector<glm::mat4> matrices;
};