#include "flockApp.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofConstants.h"

#include "ofxVec3f.h"

#include "CBox.h"
#include "CFlock.h"

//--------------------------------------------------------------
void flockApp::setup()
{
  box = new CBox(500.0, 500.0, 500.0);
  
  // build some flocks for the boids to belong to
  // now assign the boids to the flocks
  
  Flocks[0] = new CFlock(10);
  Flocks[1] = new CFlock(4);
  Flocks[2] = new CFlock(13);
  Flocks[3] = new CFlock(2);
  Flocks[4] = new CFlock(1);

  mouseDownX=mouseDownY=0;
  inMouseDrag = false;
}

//--------------------------------------------------------------
void flockApp::update()
{
  for (int i = 0; i < MAX_FLOCKS; i++) {
    Flocks[i]->Update();
  }
  
  if(inMouseDrag)
    {
      ofxVec3f screenVec(mouseX-mouseDownX,mouseY-mouseDownY,0.0);
      screenVec.limit(ofGetHeight()/2);
      float angle = (10.0 / 60.0) * screenVec.length() / (ofGetHeight()/2);
      screenVec.normalize();
      screenVec.set(screenVec.y,screenVec.x,0.0);
      wrld_rot = wrld_rot * ofxQuaternion(ofDegToRad(angle), screenVec);
    }
}

//--------------------------------------------------------------
void flockApp::draw()
{
  float ang=0.0;
  ofxVec3f axis;
  wrld_rot.getRotate(ang, axis);
  
  ofPushMatrix();
  
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
    ofRotate(ofRadToDeg(ang), -axis.x, axis.y, -axis.z);
  
    box->Draw();
    
    for (int i = 0; i < MAX_FLOCKS; i++) {
      Flocks[i]->Draw();
    }

  ofPopMatrix();
}

//--------------------------------------------------------------
void flockApp::exit()
{
  delete box;
  
  ofBaseApp::exit();  
}

//--------------------------------------------------------------
void flockApp::keyPressed(int key)
{

  switch (key) 
  {
    case OF_KEY_ESC: 
    exit();
    break;
    case OF_KEY_UP: 
    wrld_rot = wrld_rot * ofxQuaternion(ofDegToRad(5.0), ofxVec3f(1.0, 0.0, 0.0));
    break;
    case OF_KEY_DOWN: 
    wrld_rot = wrld_rot * ofxQuaternion(ofDegToRad(-5.0), ofxVec3f(1.0, 0.0, 0.0));
    break;
    case OF_KEY_LEFT: 
    wrld_rot = wrld_rot * ofxQuaternion(ofDegToRad(5.0), ofxVec3f(0.0, 1.0, 0.0));
    break;
    case OF_KEY_RIGHT: 
    wrld_rot = wrld_rot * ofxQuaternion(ofDegToRad(-5.0), ofxVec3f(0.0, 1.0, 0.0));
    break;
  }
}

//--------------------------------------------------------------
void flockApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void flockApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void flockApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void flockApp::mousePressed(int x, int y, int button)
{
  if(0==button)
    {
      mouseDownX = x;
      mouseDownY = y;
      inMouseDrag = true;
    }
}

//--------------------------------------------------------------
void flockApp::mouseReleased(int x, int y, int button)
{
  mouseDownX=mouseDownY=0;
  inMouseDrag = false;
}

//--------------------------------------------------------------
void flockApp::windowResized(int w, int h)
{
  
}

