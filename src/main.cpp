#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    //ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetupOpenGL(1920,1080,OF_GAME_MODE);
    //ofSetupOpenGL(1600,900,OF_GAME_MODE);

//    ofGLFWWindowSettings settings;
//    settings.width = 1600;
//    settings.height = 900;
//    settings.setGLVersion(2,1);
//    settings.windowMode = OF_GAME_MODE;
//    ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
