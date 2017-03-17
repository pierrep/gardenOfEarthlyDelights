#pragma once

#include "ofMain.h"
#include "ofxTraerPhysics.h"
#include "SubVine.h"

using namespace traer::physics;

class Vine
{

public:
    Vine();

    void setTexture(ofTexture& tex);
    void update();
    void draw();
    void drawLeaf(ofVec2f pt, ofVec2f normal, float stemWidth);
    ofVec2f updatePoint(unsigned int index, ofVec2f previousPoint);
    ofVec2f startPoint();
    ofVec2f endPoint();
    void drawLHS();
    void drawRHS();
    void reset();
    void setOrigin(Particle* p);
    void setTarget(Particle* p);
    void toggleNormals() {bDrawNormals = !bDrawNormals;}
    void toggleFill() {bFill = !bFill;}
    void grow(bool _bGrow) {bGrow = _bGrow;}
    void createSubVine(ofVec2f origin, ofVec2f target);

    vector<ofVec2f> points;
    vector<ofVec2f> normals;
    vector<float> rand;
    Particle* originParticle;
    Particle* targetParticle;
    vector<SubVine> subvines;

    ofVec2f target;
    ofVec2f origin;

    unsigned int numPoints;
    float maxStemWidth;
    float stemNarrowness;
    int numVertices;
    float leafSize;
    ofTexture leaf;
    bool bDrawNormals;
    bool bFill;
    bool bGrow;
    int flipCurve;
    float leafPhase;
    float curvature;
    bool bIsSubVine;
    int subvineLength;
};
