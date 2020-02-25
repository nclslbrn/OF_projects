#include "Walker.h"

Walker::Walker() {
    pos.set(0, 0, 0);
    size = 1;
    isStuck = false;
}
//--------------------------------------------------------------

Walker::Walker(ofVec3f _pos, float _size) {
    pos = _pos;
    size = _size;
    isStuck = false;
}
//--------------------------------------------------------------

void Walker::walk() {
    ofVec3f randomPoint = ofVec3f(
        ofRandom(ofGetWidth()),
        ofRandom(ofGetHeight()),
        ofRandom(ofGetHeight()));
    pos.interpolate(randomPoint, 0.5);
}
//--------------------------------------------------------------
int Walker::isCloseTo(deque<Walker> walkers) {
    for (int i = 0; i < walkers.size(); i++) {
        float distance = pos.distance(walkers[i].pos);
        float minDist = (size + walkers[i].size) / 2;
        if (distance <= minDist) {
            isStuck = true;
            return i;
        }
    }
    return -1;
}
