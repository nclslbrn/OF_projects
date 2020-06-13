#include "arc.h"

Arc::Arc(float depth) {
    float newAngle = 0.0f;
    float currentAngle = 0.0f;
    deque<float> angles;
    angles.clear();

    while (currentAngle <= glm::two_pi<float>()) {
        newAngle = (ofRandom(1.0f) * glm::pi<float>()) / 6.0f;
        angles.push_back(newAngle);
        currentAngle += newAngle;
    }
    this->depth = depth;
    this->angles = angles;
    this->initialAngle = ofRandom(1.0f) * glm::two_pi<float>();
}
//--------------------------------------------------------------
void Arc::drawFromXandYRot(
    ofVec3f point, float xRot, float yRot,
    float radius, float t, ofVec3f nextpoint) {
    float theta1 = glm::two_pi<float>() * t;
    float res = glm::two_pi<float>() / 64.0f;

    ofPushStyle();
    ofSetLineWidth(1);
    ofSetColor(200);
    for (float theta2 = 0; theta2 < glm::two_pi<float>(); theta2 += res) {
        ofVec3f pt1 = ofVec3f(
            point.x + (radius * glm::sin(theta1 + theta2) * xRot),
            point.y + (radius * glm::sin(theta1 + theta2) * yRot),
            point.z + (radius * glm::cos(theta1 + theta2)));
        ofVec3f pt2 = ofVec3f(
            point.x + (radius * glm::sin(theta1 + theta2 + res) * xRot),
            point.y + (radius * glm::sin(theta1 + theta2 + res) * yRot),
            point.z + (radius * glm::cos(theta1 + theta2 + res)));
        ofVec3f pt3 = ofVec3f(
            nextpoint.x + (radius * glm::sin(theta1 + theta2) * xRot),
            nextpoint.y + (radius * glm::sin(theta1 + theta2) * yRot),
            nextpoint.z + (radius * glm::cos(theta1 + theta2)));

        pt1 = getNoisedVector(pt1, t);
        pt2 = getNoisedVector(pt2, t);
        pt3 = getNoisedVector(pt3, t);

        ofDrawLine(pt1.x, pt1.y, pt1.z, pt2.x, pt2.y, pt2.z);
        ofDrawLine(pt1.x, pt1.y, pt1.z, pt3.x, pt3.y, pt3.z);
    }
    ofPopStyle();

    theta1 += this->initialAngle;
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(5);

    for (int k = 0; k < this->angles.size() - 1; k += 2) {
        ofBeginShape();
        for (float theta2 = 0; theta2 <= this->angles[k] - this->angles[k + 1]; theta2 += res) {
            ofVec3f pt = ofVec3f(
                point.x + (radius * glm::sin(theta1 + theta2) * xRot),
                point.y + (radius * glm::sin(theta1 + theta2) * yRot),
                point.z + (radius * glm::cos(theta1 + theta2)));
            pt = getNoisedVector(pt, t);
            ofVertex(pt.x, pt.y, pt.z);
        }
        ofEndShape(false);
        theta1 += angles[k + 1];
    }
    ofPopStyle();
}
//--------------------------------------------------------------
ofVec3f Arc::getNoisedVector(ofVec3f orig, float t) {
    float sample = 24.0;

    float noise = ofNoise(
        orig.y * sample / this->noiseScale,
        orig.z * sample / this->noiseScale,
        glm::cos(t * glm::two_pi<float>()),
        glm::sin(t * glm::two_pi<float>()));

    return ofVec3f(
        orig.x,
        orig.y + glm::sin(noise) * this->noiseRadius,
        orig.z + glm::cos(noise) * this->noiseRadius);
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
