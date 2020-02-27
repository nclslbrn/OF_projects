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
    int randomDirection = (int)ofRandom(1) % 6 + 1;
    switch (randomDirection) {
        case 0:
            pos.x += size;
            break;
        case 1:
            pos.x -= size;
            break;
        case 2:
            pos.y += size;
            break;
        case 3:
            pos.y -= size;
            break;
        case 4:
            pos.z += size;
            break;
        case 5:
            pos.z -= size;
            break;
    }
}

//--------------------------------------------------------------
int Walker::isCloseTo(deque<Walker> tree) {
    for (int i = 0; i < tree.size(); i++) {
        float minDist = (size + tree[i].size) / 2;
        float dx = std::abs(tree[i].pos.x - pos.x);
        float dy = std::abs(tree[i].pos.y - pos.y);
        float dz = std::abs(tree[i].pos.z - pos.z);

        if (dx <= minDist && dy <= minDist && dz <= minDist) {
            isStuck = true;
            return i;
            break;
        }
    }
    return -1;
}
