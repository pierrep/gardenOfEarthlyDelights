#include "Node.h"

#define NODE_SIZE 20
#define EDGE_LENGTH 30
#define EDGE_STRENGTH 0.05
#define EDGE_DAMPING 0.05
#define SPACER_STRENGTH 2000


Node::Node(shared_ptr<ParticleSystem> _physics, shared_ptr<Data> _data, int _type)
{
    physics = _physics;
    data = _data;
    addNode();
    type = _type;
}

void Node::setup(shared_ptr<ParticleSystem> _physics)
{


}


void Node::draw()
{
    ofSetColor(255);
    //ofDrawCircle( p->position, NODE_SIZE);
    if(data->categories[0].isAllocated())
    {
        ofSetRectMode(OF_RECTMODE_CENTER);
        data->categories.at(type).draw(p->position,NODE_SIZE*2,NODE_SIZE*2);
    }
}


void Node::addNode()
{
    p = physics->makeParticle();
    Particle* q = physics->getParticle( (int)ofRandom( 0, physics->numberOfParticles()-1) );
    while ( (q == p) && (physics->numberOfParticles() > 1))
      q = physics->getParticle( (int)ofRandom( 0, physics->numberOfParticles()-1) );
    addSpacersToNode( p, q );
    makeEdgeBetween( p, q );
    p->position.set( q->position.x + ofRandom( -1, 1 ), q->position.y + ofRandom( -1, 1 ), 0 );
}

//--------------------------------------------------------------
void Node::makeEdgeBetween( Particle* a, Particle* b )
{
  physics->makeSpring( a, b, EDGE_STRENGTH, EDGE_DAMPING, EDGE_LENGTH );
}

//--------------------------------------------------------------
void Node::addSpacersToNode( Particle* p, Particle* r )
{
  for ( int i = 0; i < physics->numberOfParticles(); ++i )
  {
    Particle* q = physics->getParticle( i );
    if ( (p != q) && (p != r) )
      physics->makeAttraction( p, q, -SPACER_STRENGTH, NODE_SIZE*3 );
  }
}
