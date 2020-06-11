#include "arc.h"

Arc::Arc() {
    float newAngle = 0.0f;
    float currentAngle = 0.0f;

    deque<float> angles;
    angles.clear();

    while (currentAngle <= glm::two_pi<float>()) {
        newAngle = (ofRandom(1.0f) * glm::pi<float>()) / 4.0f;
        angles.push_back(newAngle);
        currentAngle += newAngle;
    }
    this->angles = angles;
    this->initialAngle = ofRandom(1.0f) * glm::two_pi<float>();
}

deque<float> Arc::getArcAngles() {
    return this->angles;
}
float Arc::getInitialArcAngle() {
    return this->initialAngle;
}