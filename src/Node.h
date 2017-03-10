#ifndef NODE_H
#define NODE_H

#include "ofMain.h"
#include "ofxTraerPhysics.h"
#include "Data.h"

using namespace traer::physics;

class Node
{
public:
    Node(shared_ptr<ParticleSystem> _physics, shared_ptr<Data> _data, int _type);
    void setup(shared_ptr<ParticleSystem> _physics);
    void draw();
    void addNode();
    void makeEdgeBetween( Particle* a, Particle* b );
    void addSpacersToNode( Particle* p, Particle* r );


private:
    ofImage img;
    Particle* p;
    shared_ptr<ParticleSystem> physics;

    shared_ptr<Data> data;
    int type;
};

#endif // NODE_H
