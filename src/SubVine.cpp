#include "SubVine.h"


//--------------------------------------------------------------
SubVine::SubVine()
{
    numPoints = 8;
    maxStemWidth = ofRandom(3,5);
    stemNarrowness = 0.5f;
    bDrawNormals = false;
    bFill = true;
    target = ofVec2f::zero();
    origin = ofVec2f::zero();
    leafSize = 0.2f;
    if(ofRandom(0,1) < 0.5f) {
        flipCurve = 1;
    } else {
        flipCurve = -1;
    }
    bGrow = false;
    leafPhase = ofRandom(0,100);
    curvature = ofRandom(240,1200);
}

//--------------------------------------------------------------
void SubVine::setTexture(ofTexture& tex)
{
    leaf = tex;

}

//--------------------------------------------------------------
void SubVine::update()
{

    if(bGrow) {
        points[0] = origin;
        if(points.size() <= numPoints) {

            for(unsigned int i = 1; i < points.size();i++) {
                points[i] = updatePoint(i,points[i-1]);
            }

            ofVec2f pt = updatePoint(points.size(),points.back());
            points.push_back(pt);

        }
        else bGrow = false;
    }
    else
    { /* Update points */
        points[0] = origin;
        for(unsigned int i = 1; i < points.size();i++) {
            points[i] = updatePoint(i,points[i-1]);
        }
       // points[points.size()-1] = target;
    }

}

ofVec2f SubVine::updatePoint(unsigned int index, ofVec2f previousPoint)
{
    ofVec2f towards = (target - origin)/numPoints;

    ofVec2f pt = previousPoint + towards;
    float angle = sin(flipCurve*(float)index/2.0f)*(curvature/(float)numPoints);

    pt = pt.getRotated(angle,previousPoint);
    return pt;
}

//--------------------------------------------------------------
void SubVine::draw()
{
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(29,150,69);
    if(bFill) ofFill();
    else ofNoFill();

    ofBeginShape();
    ofVec2f start = startPoint();
    ofCurveVertex(start.x,start.y);
    ofCurveVertex(start.x,start.y);

    drawLHS();
    ofCurveVertex(points[points.size()-1].x,points[points.size()-1].y);
    drawRHS();

    ofVec2f end = endPoint();
    ofCurveVertex(end.x,end.y);
    ofCurveVertex(start.x,start.y);
    ofCurveVertex(start.x,start.y);
    ofEndShape();

    if(bDrawNormals) {
        ofPushStyle();
        ofSetColor(0,0,0);
        for (unsigned i = 0; i < points.size(); i++){
            ofDrawCircle(points[i].x, points[i].y,4);
        }
        ofPopStyle();
    }

    ofPopStyle();
}

//--------------------------------------------------------------
void SubVine::drawLHS()
{
    for(unsigned i = 1; i < (points.size()-1);i++)
    {
            ofVec2f vec1 = ofVec2f(points[i] - points[i-1]);
            ofVec2f normal = vec1.getPerpendicular();
            float stemWidth = ofMap(i,1,points.size()-1,1,stemNarrowness);
            ofVec2f pt = points[i] - (normal*stemWidth*maxStemWidth);
            if(bDrawNormals) {
                ofPushStyle();
                ofFill();
                ofSetColor(255,0,0);
                ofDrawCircle(pt.x, pt.y,4);
                ofPopStyle();
            }
            ofCurveVertex(pt.x,pt.y);

            if(i%6 == 0) {
                drawLeaf(pt,normal, stemWidth);
            }

            if(i == 15) {
                //drawLeaf(pt,normal, stemWidth);
            }

    }
}

//--------------------------------------------------------------
void SubVine::drawRHS()
{
    if(points.size() > 1) { //don't calculate normal if there's only one point
        for(unsigned i = points.size()-2; i > 0;i--)
        {
            ofVec2f vec1 = ofVec2f(points[i-1] - points[i]);
            ofVec2f normal = vec1.getPerpendicular();
            float stemWidth = ofMap(i,1,points.size()-1,1,stemNarrowness);
            ofVec2f pt = points[i] - (normal*stemWidth*maxStemWidth);
            if(bDrawNormals) {
                ofPushStyle();
                ofFill();
                ofSetColor(255,0,0);
                ofDrawCircle(pt.x, pt.y,4);
                ofPopStyle();
            }
            ofCurveVertex(pt.x,pt.y);

            if(i == 4) {
                drawLeaf(pt, normal, stemWidth);
            }
            if(i == 20) {
                drawLeaf(pt, normal, stemWidth);
            }
        }
    }
}

//--------------------------------------------------------------
void SubVine::drawLeaf(ofVec2f pt, ofVec2f normal, float stemWidth)
{
    ofPushMatrix();
    ofPushStyle();
    ofVec2f up = ofVec2f(0,1);
    float angle = up.angle(normal);
    ofTranslate(pt.x,pt.y);

    // translate forward a bit to avoid having the leaf based be exposed
    ofTranslate(normal*(stemWidth*maxStemWidth));

    ofRotateZ(angle);

    ofRotateZ(sin(ofGetFrameNum()/50.0f + leafPhase)*10);

    ofEnableAlphaBlending();
    ofSetColor(255);
    leaf.setAnchorPercent(0.5f,0);
    leaf.draw(0,0,leaf.getWidth()*leafSize*stemWidth,leaf.getHeight()*leafSize*stemWidth);
    ofDisableAlphaBlending();
    ofPopStyle();
    ofPopMatrix();
}


//--------------------------------------------------------------
ofVec2f SubVine::startPoint()
{
    ofVec2f vec1 = ofVec2f(target - origin);
    ofVec2f normal = vec1.getPerpendicular();
    ofVec2f pt = origin - (normal*maxStemWidth);
    if(bDrawNormals) {
        ofPushStyle();
        ofFill();
        ofSetColor(255,0,0);
        ofDrawCircle(pt.x, pt.y,4);
        ofPopStyle();
    }
    return pt;
}

//--------------------------------------------------------------
ofVec2f SubVine::endPoint()
{
    ofVec2f vec2 = ofVec2f(target - origin);
    ofVec2f normal2 = vec2.getPerpendicular();
    ofVec2f pt = origin - (-normal2*maxStemWidth);
    if(bDrawNormals) {
        ofPushStyle();
        ofFill();
        ofSetColor(255,0,0);
        ofDrawCircle(pt.x, pt.y,4);
        ofPopStyle();
    }
return pt;
}

//--------------------------------------------------------------
void SubVine::setTarget(ofVec2f _target)
{
    target = _target;
}


//--------------------------------------------------------------
void SubVine::setOrigin(ofVec2f _origin)
{
    origin = _origin;
    points.push_back(origin);
}

//--------------------------------------------------------------
void SubVine::reset()
{
    points.clear();
    points.push_back(origin);
}
