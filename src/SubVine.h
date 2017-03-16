#pragma once

#include "ofMain.h"


class SubVine
{

public:
    SubVine();

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
    void setOrigin(ofVec2f origin);
    void setTarget(ofVec2f target);
    void updateOrigin(ofVec2f _origin) {origin = _origin;}
    void updateTarget(ofVec2f _target) {target = _target;}
    void toggleNormals() {bDrawNormals = !bDrawNormals;}
    void toggleFill() {bFill = !bFill;}
    void grow(bool _bGrow) {bGrow = _bGrow;}

    vector<ofVec2f> points;
    vector<ofVec2f> normals;
    vector<float> rand;

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
};
