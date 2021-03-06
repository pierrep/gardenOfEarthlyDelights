#pragma once

#include "ofMain.h"

#include <float.h>

#include "ofxTraerPhysics.h"
#include "ofxVideoRecorder.h"
#include "ofxOsc.h"
#include "ofxTileSaver.h"
#include "ofxXmlSettings.h"

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

        void addNode(int type, string text, bool addtoXml);
        void drawSprings();
        void drawVines();
        void updateOsc();
        void reset();

        enum State {ACTIVE,IDLE};
        State state;
        unsigned long curTime;
        unsigned long prevTime;
        unsigned long active_time;
        unsigned long idle_time;

        shared_ptr<Data> data;
        shared_ptr<ParticleSystem> physics;
        vector<Node> nodes;
        vector<Vine> vines;

        /* recording */
        ofxVideoRecorder    vidRecorder;
        bool bRecording;
        string fileName;
        string fileExt;
        ofxTileSaver tileSaver;
        bool bSaveBigImage;

        ofFbo fbo;
        ofCamera cam;
        ofxOscReceiver oscReceiver;

        bool bShowFps;

        ofImage leaf;
        ofImage sun;
        ofTrueTypeFont font;
        ofTrueTypeFont titleFont;
        ofxXmlSettings xml;

        bool bLoadXml;
        int numNodes;
        int nodeIndex;
        unsigned int max_nodes;


};
