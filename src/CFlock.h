//Copyright (C) Brian Walsh, 2011
//Portions Copyright (C) Steven Woodcock, 2000
//*********************************************************************
// Name:     CFlock.h
// Purpose:  Class definitions and method prototypes for the flocks
//           themselves.
//*********************************************************************

#ifndef _CFLOCK_H
#define _CFLOCK_H

//
// includes
//

//#include "defaults.h"
//#include "util.h"

#include "CBoid.h"

#define MAX_FLOCKS 5
#define DEFAULT_PERCEPTION_RANGE    8.0f
#define KEEP_AWAY_DIST              6.0f
#define SEPARATION_DIST             3.0f

//
// class definition
//

class CFlock 
{

   public:

      ///////////////////
      // static variables
      ///////////////////

      // number of flocks

      static int FlockCount;

      // list of flocks

      static CFlock * ListOfFlocks[MAX_FLOCKS];

      ///////////////////////////////
      // constructors and destructors
      ///////////////////////////////

      // Constructor.
      // Creates a new flock.

      CFlock (void);

      // Destructor.

      ~CFlock (void);

      /////////////////////
      // flocking functions
      /////////////////////

      // Update.
      // Updates all members of a flock.

      void Update (void);

      //////////////////////
      // rendering functions
      //////////////////////

      void Draw (float x, float y);

      //////////////////////////
      // miscellaneous functions
      //////////////////////////

      // AddTo.
      // Adds the indicated boid to the flock.

      void AddTo (CBoid * boid);

      // GetCount.
      // Returns the # of boids in a given flock.

      int GetCount (void);

      // GetFirstMember.
      // Returns a pointer to the first boid 
	  // in a given flock (if any).

      CBoid * GetFirstMember (void);

      // RemoveFrom.
      // Removes the indicated boid from the flock.

      void RemoveFrom (CBoid * boid);

   private:

      int     m_id;                    // id of this flock

      int     m_num_members;           // number of boids in this flock

      CBoid   *m_first_member;         // pointer to first member

};

#endif