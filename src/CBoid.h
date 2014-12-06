//Copyright (C) Brian Walsh, 2011
//Portions Copyright (C) Steven Woodcock, 2000
//*********************************************************************
// Name:     CBoid.h
// Purpose:  Class definitions and method prototypes for individual
//           boids (flock members).
//*********************************************************************

#ifndef _CBOID_H
#define _CBOID_H

//
// includes
//

#include "ofVectorMath.h"
#include "flockApp.h"

#define MAX_FRIENDS_VISIBLE MAX_BOIDS/2

//
// class definition
//

class CBoid {
  private:
  
   public:

      ///////////////////
      // static variables
      ///////////////////

      // visible friends list (work space reused by each boid)

      static CBoid * VisibleFriendsList[MAX_FRIENDS_VISIBLE];

      ///////////////////////////////
      // constructors and destructors
      ///////////////////////////////

      // Constructor #1.
      // Creates an individual boid with randomized position, 
      // velocity, and orientation.

      CBoid (int id_v);

      // Constructor #2.
      // Creates an individual boid with specific position, 
      // velocity, and orientation.

      CBoid (int id_v,
             ofVec3f * pos_v,
             ofVec3f * vel_v,
             ofVec3f * ang_v);

      // Destructor

      ~CBoid (void);

      //////////////////////////
      // public flocking methods
      //////////////////////////

      // FlockIt.
      // Used for frame-by-frame updates; no time 
      // deltas on positions.

      void FlockIt (int flock_id, CBoid *first_boid);

      //////////////////////////
      // miscellaneous functions
      //////////////////////////

      // AddToVisibleList.
      // This visibility list is regenerated for each member each update cycle,
      // and acts much like a push-down queue; the latest member added to the
      // list becomes the first one when the list is sequentially accessed.
      // Mostly I did this for speed reasons, as this allows for fast inserts
      // (and we don't delete from this list, we just rebuild it each update cycle).

      void AddToVisibleList (CBoid *ptr);

      // ClearVisibleList.
      // Clears the visibility list.

      void ClearVisibleList (void);

      // GetOrient.
      // Returns the orientation of the boid in question.

      ofVec3f * GetOrient (void);

      // GetPos.
      // Returns the position of the boid in question.

      ofVec3f * GetPos (void);

      // GetNext.
      // Returns the "next" pointer of the invoking member.

      CBoid * GetNext(void);

      // LinkOut.
      // Removes a member from a list.

      void LinkOut (void);

      // SetNext.
      // Set the "next" pointer of an individual member.

      void SetNext (CBoid *ptr);

      // SetPrev.
      // Set the "prev" pointer of an individual member.

      void SetPrev (CBoid *ptr);

   private:

      ///////////////
      // data members
      ///////////////

      // supplied with constructor(s)

      short    m_id;                         // member individual ID

      float    m_perception_range;           // how far member can see

      ofVec3f   m_pos;                       // position of member (in meters)
      ofVec3f   m_vel;                       // velocity of member (meters/sec)
      ofVec3f   m_ang;                       // orientation of member
                                             // (right-handed coordinate system)
      // computed

      float    m_speed;                      // overall speed of member

      short    m_num_flockmates_seen;        // # of flockmates this member sees
      short    m_num_enemies_seen;           // # of enemies this member sees

      CBoid    *m_nearest_flockmate;         // pointer to nearest flockmate (if any)
   
      CBoid    *m_nearest_enemy;             // pointer to nearest enemy (if any)

      float    m_dist_to_nearest_flockmate;  // distance to nearest flockmate
                                             // (if any), in meters

      float    m_dist_to_nearest_enemy;      // distance to nearest enemy
                                             // (if any), in meters

      ofVec3f   m_oldpos;                     // last position
      ofVec3f   m_oldvel;                     // last velocity

      CBoid    *m_next;                      // pointer to next flockmate
      CBoid    *m_prev;                      // pointer to previous flockmate

      ///////////////////
      // flocking methods
      ///////////////////

      // Cruising.
      // Generates a vector indicating how a flock boid would
      // like to move, if it were all up to him and he was under
      // no other influences of any kind.

      ofVec3f Cruising (void);

      // FleeEnemies.
      // Generates a vector for a flock boid to avoid the 
      // nearest enemy (boid of a different flock) it sees.

      ofVec3f FleeEnemies (void);

      // KeepDistance.
      // Generates a vector for a flock boid to maintain his
      // desired separation distance from the nearest flockmate he sees.

      ofVec3f KeepDistance (void);

      // MatchHeading.
      // Generates a vector for a flock boid to try
      // to match the heading of its nearest flockmate.

      ofVec3f MatchHeading (void);

      // SeeEnemies.
      // Determines which enemy flock boids a given flock boid can see.

      int SeeEnemies (int flock_id);

      // SeeFriends.
      // Determines which flockmates a given flock boid can see.

      int SeeFriends (CBoid *first_boid);

      // SteerToCenter.
      // Generates a vector to guide a flock boid towards
      // the "center of mass" of the flockmates he can see.

      ofVec3f SteerToCenter (void);

      // WorldBound.
      // Implements a world boundary so that flocks don't fly
      // infinitely far away from the camera, instead remaining 
      // in a nice viewable area.  It does this by wrapping flock
      // boids around to the other side of the world, so (for example)
      // they move out the right and return on the left.

      void WorldBound (void);

      //////////////////////////
      // miscellaneous functions
      //////////////////////////

      // AccumulateChanges.
      // Adds vector values in changes into the accumumlator vector.
      // Returns magnitude of accumulator vector after adding changes.

      float AccumulateChanges (ofVec3f &accumulator,
                               ofVec3f changes);

      // CanISee.
      // Determine whether a given invoking boid can see the boid in question.
      // Returns the distance to the boid.

      float CanISee (CBoid *ptr);

      // ComputeRPY.
      // Computes the roll/pitch/yaw of the flock boid based on its
      // latest velocity vector changes.  Roll/pitch/yaw are stored in
      // the "ang" data member as follows:
      //   pitch is about the x axis
      //   yaw is about the y axis
      //   roll is about the z axis
      // All calculations assume a right-handed coordinate system:
      //   +x = through the left side of the object
      //   +y = up
      //   +z = through the nose of the model

      void ComputeRPY (void);
};

#endif
