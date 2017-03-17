#ifndef DATA_H
#define DATA_H

#include "ofMain.h"
#include "ofxThreadedImageLoader.h"
#include "ofxVideoBufferPlayer.h"

class Data
{
public:
    Data();
    void load();

    vector<ofImage> categories;
    //vector<shared_ptr<ofxVideoBufferPlayer>> animations;
    vector<shared_ptr<ofVideoPlayer>> animations;
    shared_ptr<ofxThreadedImageLoader> imageLoader;

    vector<ofSoundPlayer> sounds;
    float appWidth, appHeight;
};

#endif // DATA_H
