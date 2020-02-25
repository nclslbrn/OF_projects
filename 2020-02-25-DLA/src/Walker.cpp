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
    pos.x++;
    break;
  case 1:
    pos.x--;
    break;
  case 2:
    pos.y++;
    break;
  case 3:
    pos.y--;
    break;
  case 4:
    pos.z++;
    break;
  case 5:
    pos.z--;
    break;
  }
}

//--------------------------------------------------------------
int Walker::isCloseTo(deque<Walker> walkers) {
  for (int i = 0; i < walkers.size(); i++) {
    float minDist = (size + walkers[i].size) / 2;
    float dx = std::abs(pos.x - walkers[i].pos.x);
    float dy = std::abs(pos.y - walkers[i].pos.y);
    float dz = std::abs(pos.z - walkers[i].pos.z);

    if (dx <= minDist && dy <= minDist && dz <= minDist) {
      isStuck = true;
      return i;
      break;
    }
  }
  return -1;
}
