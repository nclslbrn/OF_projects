#include "FrameMesh.h"

FrameMesh::FrameMesh() {}
FrameMesh::FrameMesh(ofPixels framePixels, int threshold, float scale, ofVec2f texcoord) {
    pixels = framePixels;
    int numChannels = pixels.getNumChannels();
    for (int x = 0; x < pixels.getWidth(); x++) {
        for (int y = 0; y < pixels.getHeight(); y++) {
            int pixId = y * pixels.getWidth() + x;
            int red = pixels[pixId * numChannels + 0];
            int green = pixels[pixId * numChannels + 1];
            int blue = pixels[pixId * numChannels + 2];
            if (
                red > threshold &&
                green > threshold &&
                blue > threshold) {
                int z = round(((red + blue + green) / 765.0f) * pixels.getHeight() * -0.15);
                particles.push_back(
                    {{x, y, z, 1},
                     {0.0, 0.0, 0.0, 0.0},
                     {red, green, blue}});
            }
        }
    }
    matrices.resize(particles.size());
    buffer.allocate();
    buffer.bind(GL_TEXTURE_BUFFER);
    buffer.setData(matrices, GL_STREAM_DRAW);
    tex.allocateAsBufferTexture(buffer, GL_RGBA32F);
    mesh = ofMesh::box(50, 50, 50, 1, 1, 1);
    // mesh = ofMesh::plane(75, 75, 2, 2);
    // mesh.disableNormals();
    // mesh.enableTextures();
    // mesh = ofMesh::plane(100, 100, 2, 2);
    mesh.addTexCoord(ofVec2f(0, 0));
    mesh.addTexCoord(ofVec2f(1.0, 0));
    mesh.addTexCoord(ofVec2f(1.0, 1.0));
    mesh.addTexCoord(ofVec2f(0, 1.0));
    mesh.addTexCoord(ofVec2f(0, 0));
    mesh.setUsage(GL_STATIC_DRAW);
    mesh.getColors().resize(matrices.size());
    for (size_t i = 0; i < mesh.getColors().size(); i++) {
        mesh.getColors()[i] = ofColor(
            particles[i].color[0],
            particles[i].color[1],
            particles[i].color[2]);
    }

    mesh.getVbo().setAttributeDivisor(ofShader::COLOR_ATTRIBUTE, 1);
    for (size_t i = 0; i < matrices.size(); i++) {
        ofNode node;
        glm::vec3 pos(
            (particles[i].pos[0] / pixels.getWidth()) - 0.5,
            (particles[i].pos[1] / pixels.getHeight()) - 0.5,
            (particles[i].pos[2] / pixels.getHeight()) - 0.5);

        pos[0] *= ofGetWidth() * scale;
        pos[1] *= ofGetHeight() * scale;
        pos[2] *= ofGetHeight() * scale;

        node.setPosition(pos);
        node.setOrientation(pos);
        matrices[i] = node.getLocalTransformMatrix();
    }
    buffer.updateData(0, matrices);
}

void FrameMesh::drawPoints() {
    mesh.drawInstanced(OF_MESH_POINTS, matrices.size());
}
void FrameMesh::drawWireframe() {
    mesh.drawInstanced(OF_MESH_WIREFRAME, matrices.size());
}
void FrameMesh::drawFaces() {
    mesh.drawInstanced(OF_MESH_FILL, matrices.size());
}

bool FrameMesh::isTexAllocated() {
    return tex.isAllocated();
}

ofTexture FrameMesh::getTexture() {
    return tex;
}

int FrameMesh::getParticleNum() {
    return particles.size();
}
int FrameMesh::getWidth() {
    return pixels.getWidth();
}
int FrameMesh::getHeight() {
    return pixels.getHeight();
}