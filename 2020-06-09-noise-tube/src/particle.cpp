#include "particle.h"

Particle::Particle() {
    this->initPos = ofVec3f(0, 0, 0);
}
Particle::Particle(
    ofVec3f initPos,
    float width,
    float height,
    float radius) {
    initRot = ofVec3f(
        glm::two_pi<float>() * ofRandom(1),
        glm::two_pi<float>() * ofRandom(1),
        glm::two_pi<float>() * ofRandom(1));
    initPos.x += ofRandomf() * radius;
    initPos.y += ofRandomf() * radius;
    this->initPos = initPos;
    this->plane.set(width, height);
    init();
}
//--------------------------------------------------------------

void Particle::init() {
    this->plane.setPosition(this->initPos);
    this->plane.rotateRad(initRot.x, glm::vec3(1, 0, 0));
    this->plane.rotateRad(initRot.y, glm::vec3(0, 1, 0));
    this->plane.rotateRad(initRot.z, glm::vec3(0, 0, 1));
}
//--------------------------------------------------------------

void Particle::update() {
}
//--------------------------------------------------------------

void Particle::draw() {
    this->plane.draw();
}
