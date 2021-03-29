#pragma once

#include <atomic>

#include "FrameMesh.h"
#include "ofMain.h"

class LiveFrameReader : public ofThread {
   public:
    ~LiveFrameReader() {
        stop();
        waitForThread(false);
    }
    void setup() {
        // memory allocation here
        start();
    }
    void start() {
        startThread();
    }
    void stop() {
        std::unique_lock<std::mutex> lck(mutex);
        stopThread();
        condition.notify_all();
    }
    void threadedFunction() {
        // start

        while (isThreadRunning()) {
            std::unique_lock<std::mutex> lock(mutex);
            frame.compute();
            condition.wait(lock);
        }

        // done
    }
    void update(ofPixels pixels, int threshold, float scale, ofVec2f texcoord) {
        frame = FrameMesh(pixels, threshold, scale, texcoord);
        condition.notify_all();
    }
    FrameMesh frame;

   protected:
    std::condition_variable condition;
};