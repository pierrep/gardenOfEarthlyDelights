#include "ofApp.h"


void ofApp::exit() {
    vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);

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
    vidRecorder.setVideoBitrate("1200k");
    vidRecorder.setOutputPixelFormat("yuv420p");
    bRecording = false;
    tileSaver.init(10,20,true);
    bSaveBigImage = false;

    fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGBA,4);

    cam.setPosition(0,0,600);
    cam.enableOrtho();

    oscReceiver.setup(9000);
    font.load("fonts/Lobster.otf",30);
    titleFont.load("fonts/Lobster.otf",60);

    if( xml.loadFile("xml/current.xml") ){
        ofLogNotice() << "current.xml loaded.";
    }else{
        ofLogError() << "unable to load current.xml check data/ folder";
        xml.addTag("NODES");
    }

    bLoadXml = true;
    int bHasNodes = xml.getNumTags("NODES:NODE");
    if(bHasNodes > 0){
        //we push into the last NODES tag, this temporarirly treats the tag as the document root.
        xml.pushTag("NODES", bHasNodes-1);

            //we see how many entries we have stored in <NODE> tags
            numNodes = xml.getNumTags("NODE");
            nodeIndex = 0;
    }

    state = ACTIVE;
    active_time = 1000*30;
    idle_time = 1000*10;
    max_nodes = 40;
}

void ofApp::reset()
{
    physics->clear();
    nodes.clear();
    vines.clear();
    state = ACTIVE;
    prevTime = curTime;
}

//--------------------------------------------------------------
void ofApp::update(){
    curTime = ofGetElapsedTimeMillis();
    if(state == ACTIVE) {
        if(curTime - prevTime > active_time)
        {
            state = IDLE;
            prevTime = curTime;
        }
    } else if(state == IDLE){
        if(curTime - prevTime > idle_time)
        {
            prevTime = curTime;
            if(nodes.size() > 0) {
                for(int i = 0; i < nodes.size();i++)
                {
                    nodes.at(i).currentNode = false;
                }
                unsigned int idx = ofRandom(0,nodes.size());

                nodes.at(idx).currentNode = true;
                for(int i = 0; i < data->sounds.size();i++)
                {
                    data->sounds[i].stop();
                }
                int type = nodes.at(idx).getType();
                if(data->sounds[type].isLoaded()) {

                    data->sounds[type].play();
                }
            }

        }
    }

    if((nodes.size() > max_nodes) && (state == IDLE) && (!bLoadXml)) {
        string name = "xml/current"+ofToString(ofGetSeconds())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetHours())+"-"+ofToString(ofGetDay())+"-"+ofToString(ofGetMonth())+".xml";
        xml.saveFile(name);
        xml.clear();
        xml.addTag("NODES");
        xml.saveFile("xml/current.xml");
        reset();
    }



    if(bLoadXml){
        if(ofGetFrameNum()%120 == 0) { //second inerval between node insertions
            if(numNodes > 0) {
                //the last argument of getValue can be used to specify
                //which tag out of multiple tags you are refering to.
                int type = xml.getValue("NODE:Type", 0, nodeIndex);
                string text = xml.getValue("NODE:Text", "Nothing", nodeIndex);
                addNode(type,text,false);
                numNodes--;
                nodeIndex++;
            }
            if(numNodes == 0) {
                bLoadXml = false;
                xml.popTag();
                prevTime = curTime = ofGetElapsedTimeMillis();
            }
        }
    }

    physics->tick();
    updateOsc();

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

    if(bRecording){
        fbo.begin();
    }

    ofClear(255,255,255,255);

    cam.begin();

    if(bSaveBigImage) {
        tileSaver.begin();
        tileSaver.setCameraData(cam.getPosition(), ofVec3f::zero(), cam.getUpDir());
        ofMatrix4x4 mat = cam.getProjectionMatrix();
        double left, right, bottom, top, nearZ, farZ;
        mat.getOrtho(left,right,bottom,top, nearZ, farZ);
        tileSaver.getCurFrustum().set(left, right, bottom, top, nearZ, farZ);
    }

    ofPushMatrix();
    ofSetColor(255);
  //  sun.draw(-data->appWidth/2.0f,40);

    //drawSprings();

    drawVines();

    unsigned int index = 0;
    for ( unsigned int i = 0; i < nodes.size(); i++ )
    {
        nodes[i].draw();
        if(nodes[i].currentNode) {
            index = i;
        }
    }
    if(nodes.size()>1) {
        nodes[index].draw();
    }


    if(bSaveBigImage) {
        tileSaver.end();
    }

    ofPopMatrix();

    cam.end();

    ofPushStyle();
    ofSetColor(255,0,154);
   // titleFont.drawString("Garden of Earthly Delights",100, 100);
    for ( unsigned int i = 0; i < nodes.size(); i++ )
    {
        if(nodes[i].currentNode) {
            font.drawString(nodes[i].getText(),20,data->appHeight - 30);
        }
    }
    ofPopStyle();

    if(bRecording){
        fbo.end();
        fbo.draw(0,0);
    }

    if(bShowFps) {
        ofPushStyle();
        ofSetColor(0);
        string curState = "";
        if(state == ACTIVE) {
            curState = "ACTIVE";
        } else if (state == IDLE) {
            curState = "IDLE";
        }

        ofDrawBitmapString(curState,ofGetWidth()-200,ofGetHeight()-20);
        ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-170,ofGetHeight()-20);
        ofDrawBitmapString(ofToString(nodes.size()) +" Nodes",ofGetWidth()-100,ofGetHeight()-20);
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
void ofApp::addNode(int type, string text, bool addtoXml)
{
    for(int i = 0; i < nodes.size();i++)
    {
        nodes.at(i).currentNode = false;
    }

    Node node(physics, data, type, text);
    node.currentNode = true;
    nodes.push_back(node);

    Vine vine;
    vine.setTexture(leaf.getTexture());
    vine.setOrigin(node.getOrigin());
    vine.setTarget(node.getTarget());
    vine.grow(true);

    vines.push_back(vine);

    if(addtoXml) {
        if( xml.pushTag("NODES", 0) ){
                int tagNum = xml.addTag("NODE");
                xml.setValue("NODE:Type", type, tagNum);
                xml.setValue("NODE:Text", text, tagNum);
                xml.popTag();
                xml.saveFile("xml/current.xml");
        }
    }

    prevTime = curTime = ofGetElapsedTimeMillis();
    state = ACTIVE;

    for(int i = 0; i < data->sounds.size();i++)
    {
        data->sounds[i].stop();
    }
    if(data->sounds[type].isLoaded()) {
        if(!(data->sounds[type].isPlaying())) {
            data->sounds[type].play();
        }
    }

}

void ofApp::updateOsc()
{
    while(oscReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage(m);

        if(m.getAddress() == "/entry"){
            int type = m.getArgAsInt32(0);
            string text = m.getArgAsString(1);
            ofLogNotice() << "New entry - type: " << type << " text: " << text;
            addNode(type,text,true);
        }

    }
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
    if(key == 'i') {
        bSaveBigImage = !bSaveBigImage;
    }
    if(key =='s') {
        tileSaver.finish("frame_" + ofToString(ofGetFrameNum()) + "_high.png", false);
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
    addNode(type,"Hi my name is mouse and this is a line of text",true);
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
