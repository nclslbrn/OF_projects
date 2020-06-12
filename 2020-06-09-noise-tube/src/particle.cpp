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
        glm::two_pi<float>() * ofRandom(1.0),
        glm::two_pi<float>() * ofRandom(1.0),
        glm::two_pi<float>() * ofRandom(1.0));

    initPos.x += ofRandom(-radius, radius);
    initPos.y += ofRandom(-radius, radius);

    float a = glm::two_pi<float>() * ofRandom(1);

    this->initPos = ofVec3f(
        initPos.x,
        initPos.y + (glm::cos(a) * radius),
        initPos.z + (glm::sin(a) * radius));

    this->plane.set(width, height);
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
void Particle::drawFromXandYRot(ofVec3f point, float xRot, float yRot, float radius, float t) {
    ofVec3f currPos = point - this->initPos;
    ofVec3f currRot = this->initRot.getInterpolated(this->endRot, t);
    /*  
    this->plane.rotateRad(currRot.x, glm::vec3(1, 0, 0));
    this->plane.rotateRad(currRot.y, glm::vec3(0, 1, 0));
    this->plane.rotateRad(currRot.z, glm::vec3(0, 0, 1)); */
    this->plane.setGlobalOrientation(glm::vec3(currRot));
    this->plane.setGlobalPosition(
        ofVec3f(
            currPos.x + (radius * xRot),
            currPos.y + (radius * yRot),
            currPos.z));
    this->plane.draw();
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