#pragma once

#include "ofMain.h"

class Setting {
   public:
    int brightThreshold;
    int darkThreshold;
    int queryLigthThreshold;
    int brightnessVariation;
    std::string sourceName;
    std::string photographer;
    std::string subject;

    Setting() {}
    Setting(ofJson setting) {
        this->brightThreshold = setting["brightThreshold"];
        this->darkThreshold = setting["darkThreshold"];
        this->queryLigthThreshold = setting["queryLigthThreshold"];
        this->brightnessVariation = setting["brightnessVariation"];
        this->sourceName = setting["sourceName"];
        this->photographer = setting["photographer"];
        this->subject = setting["subject"];
    }
    int getBrightThreshold() { return this->brightThreshold; }
    int getDarkThreshold() { return this->darkThreshold; }
    int getQueryLigthThreshold() { return this->queryLigthThreshold; }
    int getBrightnessVariation() { return this->brightnessVariation; }
    string getSourceName() { return this->sourceName; }
    string getPhotographer() { return this->photographer; }
    string getSubject() { return this->subject; }
};

class ofApp : public ofBaseApp {
   public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void addLine(
        int x,
        int y,
        ofColor pixColor,
        int brightness,
        bool searchBrightPix);
    void initSettings(int frameId);
    void saveFrame();

   private:
    ofJson jsonSettings;
    vector<Setting> settings;
    Setting currSetting;
    int nextBrightY(int x, int y, int brightness);
    int nextDarkY(int x, int y, int brightness);

    ofImage source;
    ofImage modified;
    bool isSaved = false;

    int currY;
    int width, height;
    int pixStep = 2;
    int frameId = 0;
    //  "1080x1080" "800x800" "400x400"
    string sourceSize = "1080x1080";

    /*
     now on settings.json
    int brightThreshold = 190;
    int darkThreshold = 70;
    int queryLigthThreshold = 20;
    int brightnessVariation = 100;
    string sourceName = "sasha-pleshco_-tT66gwvB6FY-unsplash.jpg";
    */
};
