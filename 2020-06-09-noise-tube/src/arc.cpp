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
void Arc::drawFromXandYRot(ofVec3f point, float xRot, float yRot, float radius) {
    float theta1 = this->initialAngle;
    for (int k = 0; k < this->angles.size() - 1; k += 2) {
        ofVec3f l1 = ofVec3f(
            point.x + (radius * glm::sin(theta1 + angles[k]) * xRot),
            point.y + (radius * glm::sin(theta1 + angles[k]) * yRot),
            point.z + (radius * glm::cos(theta1 + angles[k])));
        ofVec3f l2 = ofVec3f(
            point.x + (radius * glm::sin(theta1 + angles[k + 1]) * xRot),
            point.y + (radius * glm::sin(theta1 + angles[k + 1]) * yRot),
            point.z + (radius * glm::cos(theta1 + angles[k + 1])));

        ofBeginShape();
        ofVertex(l1.x, l1.y, l1.z);
        ofVertex(l2.x, l2.y, l2.z);
        ofEndShape(false);

        theta1 += angles[k + 1];
    }
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
