#pragma once

#include "ofMain.h"

#include <float.h>

#include "ofxTraerPhysics.h"
#include "ofxVideoRecorder.h"

#include "Node.h"
#include "Data.h"
#include "Vine.h"

using namespace traer::physics;


class ofApp : public ofBaseApp {

    public:
        virtual ~ofApp() {}
        void exit();
        void setup();
        void update();
        void draw();

        void keyPressed  (int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        void updateCentroid();
        void addNode(int type);
        void drawSprings();
        void drawVines();

        shared_ptr<Data> data;
        shared_ptr<ParticleSystem> physics;
        vector<Node> nodes;
        vector<Vine> vines;

        float centroidX, centroidY;
        float scale;

        float appWidth, appHeight;

        ofxVideoRecorder    vidRecorder;
        bool bRecording;
        string fileName;
        string fileExt;

        ofFbo fbo;

        ofCamera cam;

        ofImage leaf;
        ofImage sun;
};
