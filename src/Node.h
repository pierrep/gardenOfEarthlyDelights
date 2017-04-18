#ifndef NODE_H
#define NODE_H

#include "ofMain.h"
#include "ofxTraerPhysics.h"
#include "Data.h"

using namespace traer::physics;

class Node
{
public:
    Node(shared_ptr<ParticleSystem> _physics, shared_ptr<Data> _data, int _type, string _text);
    void setup();
    void update();
    void draw();
    void addNode();
    void makeEdgeBetween( Particle* a, Particle* b );
    void addSpacersToNode( Particle* a, Particle* b );
    bool inBounds(Particle* a);
    Particle* getOrigin() {return q;}
    Particle* getTarget() {return p;}
    const string getText() {return text;}
    const int getType() {return type;}

    bool currentNode;

private:
    ofImage img;
    Particle* p;
    Particle* q;
    shared_ptr<ParticleSystem> physics;

    ofVec2f start;
    ofVec2f end;

    shared_ptr<Data> data;

    int appWidth, appHeight;

    int type;
    string text;

};

#endif // NODE_H
