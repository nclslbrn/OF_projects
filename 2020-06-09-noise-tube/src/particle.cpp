#include "particle.h"

Particle::Particle() {
    this->initPos = ofVec3f(0, 0, 0);
}
Particle::Particle(
    ofVec3f initPos,
    float width,
    float height,
    float radius) {
    this->initRot = ofVec3f(
        glm::two_pi<float>() * ofRandom(1.0),
        glm::two_pi<float>() * ofRandom(1.0),
        glm::two_pi<float>() * ofRandom(1.0));

    this->endRot = ofVec3f(
        glm::two_pi<float>() * ofRandom(6.0),
        glm::two_pi<float>() * ofRandom(6.0),
        glm::two_pi<float>() * ofRandom(6.0));

    float a = glm::two_pi<float>() * ofRandom(1);

    this->initPos = ofVec3f(
        initPos.x,
        initPos.y + (glm::cos(a) * radius),
        initPos.z + (glm::sin(a) * radius));

    this->plane.set(width, height, 2, 2);
    this->init();
}
//--------------------------------------------------------------

void Particle::init() {
    this->plane.setGlobalOrientation(glm::vec3(initRot));
    this->plane.setPosition(this->initPos);
}
//--------------------------------------------------------------
void Particle::updateParticle() {
}
//--------------------------------------------------------------
void Particle::drawParticle() {
    this->plane.draw();
}
//--------------------------------------------------------------
void Particle::drawFromXandYRot(
    ofVec3f point, float xRot, float yRot, float radius,
    float t, float noiseScale, float noiseRadius) {
    ofVec3f currRot = this->initRot.getInterpolated(this->endRot, t);

    float theta1 = glm::two_pi<float>() * t;

    float noise = ofNoise(
        point.y * this->plane.getWidth() / noiseScale,
        point.z * this->plane.getHeight() / noiseScale,
        glm::cos(t * glm::two_pi<float>()),
        glm::sin(t * glm::two_pi<float>()));

    ofVec3f currPos = ofVec3f(
        point.x - this->initPos.x,
        (point.y - this->initPos.y) + glm::sin(noise) * this->plane.getWidth(),
        (point.z - this->initPos.z) + glm::sin(noise) * this->plane.getHeight());
    this->plane.setGlobalOrientation(glm::vec3(currRot));

    this->plane.setGlobalPosition(
        ofVec3f(
            currPos.x + (radius * xRot),
            currPos.y + (radius * yRot),
            currPos.z));

    this->plane.drawWireframe();
}
//--------------------------------------------------------------
// GETTERS -----------------------------------------------------
//--------------------------------------------------------------
ofVec3f Particle::getParticlePos() {
    return this->plane.getGlobalPosition();
}
//--------------------------------------------------------------
ofVec2f Particle::getParticleSize() {
    return ofVec2f(this->plane.getWidth(), this->plane.getHeight());
}