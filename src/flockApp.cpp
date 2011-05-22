#include "flockApp.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofConstants.h"

#include "ofxVec3f.h"

#include "CBox.h"
#include "CBoid.h"
#include "CFlock.h"

//--------------------------------------------------------------
void flockApp::setup()
{
  box = new CBox(500.0, 500.0, 500.0);
  
  for (int i = 0; i < MAX_BOIDS; i++) 
  {
    Boids[i] = new CBoid(i);
  }
  
  // build some flocks for the boids to belong to
  
  for (int j = 0; j < MAX_FLOCKS; j++) 
  {
    Flocks[j] = new CFlock();
  }
  
  // now assign the boids to the flocks
  
  Flocks[0]->AddTo(Boids[0]);       // yes, I could have done this with some king
  Flocks[0]->AddTo(Boids[1]);       // of loop, but I wanted to show unequal sized flocks
  Flocks[0]->AddTo(Boids[2]);
  Flocks[0]->AddTo(Boids[3]);
  Flocks[0]->AddTo(Boids[4]);
  Flocks[0]->AddTo(Boids[5]);
  Flocks[0]->AddTo(Boids[6]);
  Flocks[0]->AddTo(Boids[7]);
  Flocks[0]->AddTo(Boids[8]);
  Flocks[0]->AddTo(Boids[9]);
  
  Flocks[1]->AddTo(Boids[10]);
  Flocks[1]->AddTo(Boids[11]);
  Flocks[1]->AddTo(Boids[12]);
  Flocks[1]->AddTo(Boids[13]);
  
  Flocks[2]->AddTo(Boids[14]);
  Flocks[2]->AddTo(Boids[15]);
  Flocks[2]->AddTo(Boids[16]);
  Flocks[2]->AddTo(Boids[17]);
  Flocks[2]->AddTo(Boids[18]);
  Flocks[2]->AddTo(Boids[19]);
  Flocks[2]->AddTo(Boids[20]);
  Flocks[2]->AddTo(Boids[21]);
  Flocks[2]->AddTo(Boids[22]);
  Flocks[2]->AddTo(Boids[23]);
  Flocks[2]->AddTo(Boids[24]);
  Flocks[2]->AddTo(Boids[25]);
  Flocks[2]->AddTo(Boids[26]);
  
  Flocks[3]->AddTo(Boids[27]);
  Flocks[3]->AddTo(Boids[28]);
  
  Flocks[4]->AddTo(Boids[29]);

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

