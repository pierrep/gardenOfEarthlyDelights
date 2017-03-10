#include "ofApp.h"


void ofApp::exit() {
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255);
    width = ofGetWidth();
    height = ofGetHeight();

    physics = shared_ptr<ParticleSystem>(new ParticleSystem(0, 0.2f));
    physics->clear();


    ofSetCircleResolution(30);
    data = shared_ptr<Data>(new Data());
    data->load();

    fileName = "movie";
    fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    vidRecorder.setVideoCodec("libx264");
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setOutputPixelFormat("yuv420p");
    bRecording = false;

    fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA,4);


}

//--------------------------------------------------------------
void ofApp::update(){
    physics->tick();

    updateCentroid();

    if(bRecording){
        ofPixels pix;
        fbo.readToPixels(pix);
        pix.setNumChannels(3);
        vidRecorder.addFrame(pix);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    fbo.begin();

    ofClear(255,255,255,255);

    ofPushMatrix();

    ofTranslate( width*0.5f, height*0.5f);
    ofScale(scale, scale, scale);

    ofTranslate( -centroidX, -centroidY );

    for ( int i = 0; i < physics->numberOfSprings(); ++i )
    {
        Spring* e = physics->getSpring(i);
        Particle* a = e->getOneEnd();
        Particle* b = e->getTheOtherEnd();

        ofSetLineWidth(10);
        ofSetColor(29,150,69);
        ofDrawLine( a->position, b->position );
    }

    // draw network
    for ( unsigned int i = 0; i < nodes.size(); i++ )
    {
        nodes[i].draw();
    }

    ofPopMatrix();

    fbo.end();
    fbo.draw(0,0);

}




//--------------------------------------------------------------
// calculate particles centroid & scale
void ofApp::updateCentroid()
{
  float xMax = 0.0f;
  float xMin = 2000.0f;
  float yMin = 2000.0f;
  float yMax = 0.0f;

  for ( int i = 0; i < physics->numberOfParticles(); ++i )
  {
    Particle* p = physics->getParticle( i );
    xMax = max( xMax, p->position.x );
    xMin = min( xMin, p->position.x );
    yMin = min( yMin, p->position.y );
    yMax = max( yMax, p->position.y );
  }
  float deltaX = xMax-xMin;
  float deltaY = yMax-yMin;

  centroidX = xMin + 0.5f*deltaX;
  centroidY = yMin + 0.5f*deltaY;

  // compute scale, avoid screen out
//  if ( deltaY > deltaX )
   // scale = height/(deltaY+200);
  //else
    scale = min(width/(deltaX+300),height/(deltaY+300));

    scale *= 3;
  //cout << "scale: " << scale << endl;
}

//--------------------------------------------------------------
void ofApp::addNode()
{
    Node node(physics,data, ofRandom(0,7));
    nodes.push_back(node);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if ( key == 'i' ) initialize();
    if ( key == ' ' ) addNode();

    if(key == 'r') {
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized()) {
            ofLogNotice() << "Set up video recording";
            vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, ofGetWidth(), ofGetHeight(), 30, 0, 0);
            vidRecorder.start();
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    addNode();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
