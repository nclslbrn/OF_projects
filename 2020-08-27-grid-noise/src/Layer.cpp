#include "Layer.h"

Layer::Layer(int cols, int rows, int cellSize, float noiseDepth, int z) {
    this->noiseDepth = noiseDepth;
    this->z = z;
    this->cols = cols;
    this->rows = rows;
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            this->points.push_back(ofVec2f(x, y));
        }
    }
    this->computeNoise();
}

void Layer::computeNoise() {
    this->weights.clear();
    for (ofVec2f point : this->points) {
        this->weights.push_back(ofNoise(point.x, point.y, this->noiseDepth));
    }
}

void Layer::update(float noiseDepth) {
    if (noiseDepth > -1.0f) {
        this->noiseDepth = noiseDepth;
        this->computeNoise();
    }
    this->z++;
}
vector<ofVec2f> Layer::getNearPoints(ofVec2f point) {
    vector<ofVec2f> nearPoints;
    // top
    if (point.y > 0) {
        nearPoints.push_back(ofVec2f(point.x, point.y - 1));
    }
    // top right
    if (point.y > 0 && point.x < this->cols - 1) {
        nearPoints.push_back(ofVec2f(point.x + 1, point.y - 1));
    }
    // right
    if (point.x < this->cols - 1) {
        nearPoints.push_back(ofVec2f(point.x + 1, point.y));
    }
    // bottom right
    if (point.x < this->cols - 1 && point.y < this->rows - 1) {
        nearPoints.push_back(ofVec2f(point.x + 1, point.y + 1));
    }
    // bottom
    if (point.y < this->rows - 1) {
        nearPoints.push_back(ofVec2f(point.x, point.y + 1));
    }
    // bottom left
    if (point.x >= 1 && point.y < this->rows - 1) {
        nearPoints.push_back(ofVec2f(point.x - 1, point.y + 1));
    }
    // left
    if (point.x >= 1) {
        nearPoints.push_back(ofVec2f(point.x - 1, point.y));
    }
    // top left
    if (point.x >= 1 && point.y >= 1) {
        nearPoints.push_back(ofVec2f(point.x - 1, point.y - 1));
    }
    return nearPoints;
}

vector<ofVec2f> Layer::getPoints(float threshold, int pointX, int pointY) {
    vector<ofVec2f> selection;
    if (pointX == -1 && pointY == -1) {
        for (int i = 0; i < this->points.size(); i++) {
            if (this->weights[i] && this->weights[i] <= threshold) {
                selection.push_back(this->points[i]);
            }
        }
    } else {
        vector<ofVec2f> nearPoints = this->getNearPoints(ofVec2f(pointX, pointY));
        for (ofVec2f point : nearPoints) {
            int index = point.y * this->rows + point.x;
            if (this->weights[index] && this->weights[index] <= threshold) {
                selection.push_back(point);
            }
        }
    }
    return selection;
}

int Layer::getZpos() { return this->z; }