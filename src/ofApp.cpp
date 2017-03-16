#include "ofApp.h"


void ofApp::exit() {
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::setup(){

   // ofSetFrameRate(3);
    ofBackground(255);

    appWidth = ofGetWidth();
    appHeight = ofGetHeight();
    bShowFps = false;

    physics = shared_ptr<ParticleSystem>(new ParticleSystem(0, 0.2f));
    physics->clear();


    ofSetCircleResolution(40);
    data = shared_ptr<Data>(new Data());
    data->load();

    ofDisableArbTex();
    leaf.load("leaves/leaf2.png");
    leaf.getTexture().generateMipmap();
    //ofEnableArbTex();

    sun.load("sun.png");

    fileName = "movie";
    fileExt = ".mp4"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    vidRecorder.setVideoCodec("libx264");
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setOutputPixelFormat("yuv420p");
    bRecording = false;

    fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA,4);

    cam.setPosition(0,0,600);
    cam.enableOrtho();


}

//--------------------------------------------------------------
void ofApp::update(){
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));

    physics->tick();

    updateCentroid();

    for(int i =0; i < data->animations.size();i++) {
        data->animations.at(i)->update();
    }


    for ( int i = 0; i < vines.size();i++)
    {
        vines[i].update();

    }

    if(bRecording){
        ofPixels pix;
        fbo.readToPixels(pix);
        pix.setNumChannels(3);
        vidRecorder.addFrame(pix);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    //fbo.begin();

    ofClear(255,255,255,255);

    cam.begin();

    ofPushMatrix();

    ofSetColor(255);
    sun.draw(-appWidth/2.0f,40);

    //drawSprings();

    drawVines();

    for ( unsigned int i = 0; i < nodes.size(); i++ )
    {
        nodes[i].draw();
    }
    ofPopMatrix();

    cam.end();

    //fbo.end();
    //fbo.draw(0,0);
    if(bShowFps) {
        ofPushStyle();
        ofSetColor(0);
        ofDrawBitmapString(ofToString(ofGetFrameRate()),20,ofGetHeight()-20);
        ofDrawBitmapString(ofToString(nodes.size()) +" Nodes",100,ofGetHeight()-20);
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::drawSprings()
{
    ofPushStyle();
    for ( int i = 0; i < physics->numberOfSprings(); ++i )
    {
        Spring* e = physics->getSpring(i);
        Particle* a = e->getOneEnd();
        Particle* b = e->getTheOtherEnd();

        ofSetLineWidth(10);
        ofSetColor(29,150,69);
        ofDrawLine( a->position, b->position );
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawVines()
{
    ofPushStyle();
    for ( int i = 0; i < vines.size();i++)
    {
        vines[i].draw();
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::addNode(int type)
{
    for(int i = 0; i < nodes.size();i++)
    {
        nodes.at(i).currentNode = false;
    }

    Node node(physics, data, type);
    node.currentNode = true;
    nodes.push_back(node);

    Vine vine;
    vine.setTexture(leaf.getTexture());
    vine.setOrigin(node.getOrigin());
    vine.setTarget(node.getTarget());
    vine.grow(true);

    vines.push_back(vine);

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
    scale = min(appWidth/(deltaX+300),appHeight/(deltaY+300));

    scale *= 2;
  //cout << "scale: " << scale << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    if(key =='f') {
        bShowFps = !bShowFps;
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
    int type = ofRandom(0,7);
    addNode(type);
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
