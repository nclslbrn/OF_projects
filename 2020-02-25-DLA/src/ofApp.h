#pragma once

#include "Walker.h"
#include "Branch.h"
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();
  ofVec3f randomPos();
  ofVec3f onRadius();
  deque<Walker> tree;
  deque<Branch> branches;
  deque<Walker> walkers;
  ofLight point;
  ofEasyCam cam;

  int walkerNum, steps;
  float walkerSize, decrease, radius, zRot;

};
