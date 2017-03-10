#ifndef DATA_H
#define DATA_H

#include "ofMain.h"
#include "ofxThreadedImageLoader.h"

class Data
{
public:
    Data();
    void load();

    vector<ofImage> categories;

    shared_ptr<ofxThreadedImageLoader> imageLoader;
};

#endif // DATA_H
