#include "flockApp.h"
#include "ofAppRunner.h"
#include "CBox.h"

//--------------------------------------------------------------
void flockApp::setup()
{
  box = new CBox(50.0, 50.0, 50.0);
}

//--------------------------------------------------------------
void flockApp::update()
{

}

//--------------------------------------------------------------
void flockApp::draw()
{
  box->Draw(ofGetWidth()/2,ofGetHeight()/2);
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

}

//--------------------------------------------------------------
void flockApp::keyReleased(int key)
{
  switch (key) 
  {
    case 27: 
    exit();
    break;
  }

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

}

//--------------------------------------------------------------
void flockApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void flockApp::windowResized(int w, int h)
{
  
}

