#ifndef _FLOCK_APP
#define _FLOCK_APP

// Includes
#include "ofMain.h"

#define MAX_BOIDS                    30
#define MAX_FLOCKS                    5

// Forward declarations.
class CBox;
class CFlock;
class CBoid;

class flockApp : public ofBaseApp
{
  private:
  CBox *box;
  CFlock *Flocks[MAX_FLOCKS];
  CBoid  *Boids[MAX_BOIDS];

  public:
  
  CFlock *FlockAtIndex(int i)
  {
    if (i < 0 || i >= MAX_FLOCKS)
      return NULL;
  
    return Flocks[i];
  }
  
  
	public:
		void setup();
		void update();
		void draw();
    void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

};

#endif
