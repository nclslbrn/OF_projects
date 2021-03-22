#include "FrameMesh.h"

FrameMesh::FrameMesh() {}
FrameMesh::FrameMesh(string imgPath, int threshold, float scale) {
    img.load(imgPath);
    ofPixels pixels = img.getPixels();
    int numChannels = img.getPixels().getNumChannels();
    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            int pixId = y * img.getWidth() + x;
            int red = pixels[pixId * numChannels + 0];
            int green = pixels[pixId * numChannels + 1];
            int blue = pixels[pixId * numChannels + 2];
            if (
                red > threshold &&
                green > threshold &&
                blue > threshold) {
                int z = round(((red + blue + green) / 765.0f) * img.getHeight() * -0.15);
                particles.push_back({{x, y, z, 1},
                                     {ofRandomf(), ofRandomf(), ofRandomf(), ofRandomf()},
                                     {red, green, blue}});
            }
        }
    }
    matrices.resize(particles.size());
    buffer.allocate();
    buffer.bind(GL_TEXTURE_BUFFER);
    buffer.setData(matrices, GL_STREAM_DRAW);
    tex.allocateAsBufferTexture(buffer, GL_RGBA32F);
    // we are going to use instanced drawing so we
    // only need one geometry
    mesh = ofMesh::box(50, 50, 50, 1, 1, 1);
    // mesh.setUsage(GL_STATIC_DRAW);
    mesh.setUsage(GL_DYNAMIC_DRAW);
    //mesh.setMode(OF_PRIMITIVE_POINTS);
    // we want each box to have a different color so let's add
    // as many colors as boxes
    mesh.getColors().resize(matrices.size());
    for (size_t i = 0; i < mesh.getColors().size(); i++) {
        mesh.getColors()[i] = ofColor(
            particles[i].color[0],
            particles[i].color[1],
            particles[i].color[2]);
    }
    // then we tell the vbo that colors should be used per instance by using
    // ofVbo::setAttributeDivisor
    mesh.getVbo().setAttributeDivisor(ofShader::COLOR_ATTRIBUTE, 1);
    // put box position from our image
    for (size_t i = 0; i < matrices.size(); i++) {
        ofNode node;

        //float t = (now + i * spacing) * movementSpeed;
        glm::vec3 pos(
            (particles[i].pos[0] / img.getWidth()) - 0.5,
            (particles[i].pos[1] / img.getHeight()) - 0.5,
            (particles[i].pos[2] / img.getHeight()) - 0.5);

        pos[0] *= ofGetWidth() * scale;
        pos[1] *= ofGetHeight() * scale;
        pos[2] *= ofGetHeight() * scale;

        node.setPosition(pos);
        node.setOrientation(pos);
        matrices[i] = node.getLocalTransformMatrix();
    }
    // and upload them to the texture buffer
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

ofTexture FrameMesh::getTexture() {
    return tex;
}

int FrameMesh::getParticleNum() {
    return particles.size();
}
int FrameMesh::getWidth() {
    return img.getWidth();
}
int FrameMesh::getHeight() {
    return img.getHeight();
}