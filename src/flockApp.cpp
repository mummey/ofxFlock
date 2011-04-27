#include "flockApp.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "CBox.h"
#include "CBoid.h"
#include "CFlock.h"

//--------------------------------------------------------------
void flockApp::setup()
{
  box = new CBox(500.0, 500.0, 500.0);
  
  for (int i = 0; i < MAX_BOIDS; i++) 
  {
    Boids[i] = new CBoid(i, this);
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

}

//--------------------------------------------------------------
void flockApp::update()
{
  for (int i = 0; i < MAX_FLOCKS; i++) {
    Flocks[i]->Update();
  }
  

}

//--------------------------------------------------------------
void flockApp::draw()
{
  ofPushMatrix();
  
    glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);

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

