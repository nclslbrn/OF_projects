#include "Branch.h"

Branch::Branch() {
    start.set(0, 0, 0);
    end.set(0, 0, 0);
    weight = 1.0;
}
//--------------------------------------------------------------
Branch::Branch(ofVec3f _start, ofVec3f _end, float _weight){
    start = _start;
    end = _end;
    weight = _weight;
}

//--------------------------------------------------------------
void Branch::draw() {
    ofDrawLine(start.x, start.y, start.z, end.x, end.y, end.z);
}