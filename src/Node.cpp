#include "Node.h"

#define NODE_SIZE 75
#define EDGE_LENGTH 200
#define EDGE_STRENGTH 0.05
#define EDGE_DAMPING 0.05
#define SPACER_STRENGTH 2000


Node::Node(shared_ptr<ParticleSystem> _physics, shared_ptr<Data> _data, int _type, string _text)
{
    physics = _physics;
    data = _data;
    type = _type;
    text = _text;
    appWidth = data->appWidth;
    appHeight = data->appHeight;
    currentNode = false;

    addNode();
}

void Node::setup()
{


}

void Node::update()
{

}

void Node::draw()
{
    ofPushStyle();
    ofSetColor(255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        if(currentNode) {
            if(data->animations.at(type)->isLoaded()) {
                data->animations.at(type)->setPaused(false);
                data->animations.at(type)->setSpeed(1);                
                data->animations.at(type)->draw(p->position.x,p->position.y,NODE_SIZE*2,NODE_SIZE*2);
            }            
            /*
            if(data->categories.at(type).isAllocated())
            {
                data->categories.at(type).draw(p->position,NODE_SIZE,NODE_SIZE);
            }*/
        } else {

            if(data->categories.at(type).isAllocated())
            {
                data->categories.at(type).draw(p->position,NODE_SIZE,NODE_SIZE);
                data->animations.at(type)->setPaused(true); //stop movie
            }
        }
    ofPopStyle();

}

//--------------------------------------------------------------
void Node::addNode()
{
    p = physics->makeParticle();
    q = physics->getParticle( (int)ofRandom( 0, physics->numberOfParticles()-1) );
    while ( ((q == p) || (!(inBounds(q)))) && (physics->numberOfParticles() > 1)) {
      q = physics->getParticle( (int)ofRandom( 0, physics->numberOfParticles()-1) );
    }
    addSpacersToNode( p, q );
    makeEdgeBetween( p, q );

    p->position.set( q->position.x + ofRandom( -1, 1 ), q->position.y + ofRandom( -1, 1 ), 0 );

    data->animations.at(type)->setPaused(false);
    data->animations.at(type)->setFrame(0);
    data->animations.at(type)->play();
}


//--------------------------------------------------------------
bool Node::inBounds(Particle* a)
{
    const int border = 200;
    if((a->position.x < (appWidth/2-border)) && (a->position.x > (-appWidth/2+border))) {
        if((a->position.y < (appHeight/2-border)) && (a->position.y > (-appHeight/2+border))) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------
void Node::makeEdgeBetween( Particle* a, Particle* b )
{
    physics->makeSpring( a, b, EDGE_STRENGTH, EDGE_DAMPING, EDGE_LENGTH );
}

//--------------------------------------------------------------
void Node::addSpacersToNode( Particle* a, Particle* b )
{
  for ( int i = 0; i < physics->numberOfParticles(); ++i )
  {
    Particle* c = physics->getParticle( i );
    if ( (a != c) && (a != b) )
      physics->makeAttraction( a, c, -SPACER_STRENGTH, NODE_SIZE*3 );
  }
}
