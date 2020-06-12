#include "arc.h"

Arc::Arc() {
    float newAngle = 0.0f;
    float currentAngle = 0.0f;

    deque<float> angles;
    angles.clear();

    while (currentAngle <= glm::two_pi<float>()) {
        newAngle = (ofRandom(1.0f) * glm::pi<float>()) / 6.0f;
        angles.push_back(newAngle);
        currentAngle += newAngle;
    }
    this->angles = angles;
    this->initialAngle = ofRandom(1.0f) * glm::two_pi<float>();
}
//--------------------------------------------------------------
void Arc::drawFromXandYRot(ofVec3f point, float xRot, float yRot, float radius, float t) {
    float theta1 = this->initialAngle;
    float res = 0.008f;
    float sample = radius / 12;

    for (int k = 0; k < this->angles.size() - 1; k += 2) {
        ofBeginShape();
        for (float l = 0; l <= this->angles[k] - this->angles[k + 1]; l += res) {
            ofVec3f pt = ofVec3f(
                point.x + (radius * glm::sin(theta1 + l) * xRot),
                point.y + (radius * glm::sin(theta1 + l) * yRot),
                point.z + (radius * glm::cos(theta1 + l)));

            float noise = ofNoise(
                pt.x * sample / this->noiseScale,
                pt.y * sample / this->noiseScale,
                glm::cos(t * glm::two_pi<float>()),
                glm::sin(t * glm::two_pi<float>()));
            ofVertex(
                pt.x + glm::cos(noise) * this->noiseRadius,
                pt.y + glm::sin(noise) * this->noiseRadius,
                pt.z);
        }
        ofEndShape(false);
        theta1 += angles[k + 1];
    }
}
//--------------------------------------------------------------
// SETTERS -----------------------------------------------------
//--------------------------------------------------------------
void Arc::setNoiseRadius(float noiseRadius) {
    this->noiseRadius = noiseRadius;
}
//--------------------------------------------------------------
void Arc::setNoiseScale(float noiseScale) {
    this->noiseScale = noiseScale;
}

//--------------------------------------------------------------
// GETTERS -----------------------------------------------------
//--------------------------------------------------------------

deque<float> Arc::getArcAngles() {
    return this->angles;
}
//--------------------------------------------------------------
float Arc::getInitialArcAngle() {
    return this->initialAngle;
}
