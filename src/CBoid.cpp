//Copyright (C) Brian Walsh, 2011
//Portions Copyright (C) Steven Woodcock, 2000
//*********************************************************************
// Name:     CBoid.cpp
// Purpose:  Class methods for individual boids (flock members).
//*********************************************************************

//
// includes
//

#include <stdio.h>
#include <math.h>

#include "CFlock.h"
#include "CBoid.h"
#include "CBox.h"
#include "ofMath.h"

#define MAX_SPEED                     1.0f
#define DESIRED_SPEED                 (MAX_SPEED/2)
#define MIN_URGENCY                   0.05f
#define MAX_URGENCY                   0.1f
#define MAX_CHANGE                    (MAX_SPEED * MAX_URGENCY)
#define REACT_TO_ENEMIES              TRUE
#define USE_TRUTH                     FALSE

#define GRAVITY   9.806650f  // meters/second

//
// static variable initialization
//

// visible friends list (work space reused by each flock boid)

CBoid * CBoid::VisibleFriendsList[] = {NULL};

//
// constructor and destructor methods
//

// Constructor #1.
// Creates an individual boid with randomized position and velocity.

CBoid::CBoid (int id_v)
{

   m_id               = id_v;
   m_perception_range = DEFAULT_PERCEPTION_RANGE;

   // generate random position

   m_pos.x = ofRandomuf() * CBox::WorldPtr->GetBoxWidth()/3;
   m_pos.y = ofRandomuf() * CBox::WorldPtr->GetBoxHeight()/3;
   m_pos.z = ofRandomuf() * CBox::WorldPtr->GetBoxLength()/3;

   // flip positions for greater randomness

   if (ofRandomuf() > 0.5f) m_pos.x *= -1;
   if (ofRandomuf() > 0.5f) m_pos.y *= -1;
   if (ofRandomuf() > 0.5f) m_pos.z *= -1;

   // generate random velocity

   m_vel.x = ofRandomuf();
   m_vel.z = ofRandomuf();

   // flip velocities for greater randomness

   if (ofRandomuf() > 0.5f) m_vel.x *= -1;
   if (ofRandomuf() > 0.5f) m_vel.z *= -1;

   // compute speed based on velocity

   m_speed = m_vel.length();

   // zero out orientation

   m_ang.x = m_ang.y = m_ang.z = 0;

   // other values

   m_num_flockmates_seen       = 0;
   m_nearest_flockmate         = NULL;
   m_dist_to_nearest_flockmate = INFINITY;

   m_num_enemies_seen          = 0;
   m_nearest_enemy             = NULL;
   m_dist_to_nearest_enemy     = INFINITY;

   m_next = m_prev = NULL;

}  

// Constructor #2.
// Creates an individual boid with specific position and velocity.

CBoid::CBoid (int id_v,
              ofVec3f * pos_v, ofVec3f * vel_v, ofVec3f * ang_v)
{

   m_id               = id_v;
   m_perception_range = DEFAULT_PERCEPTION_RANGE;

   m_pos = *pos_v;
   m_vel = *vel_v;
   m_ang = *ang_v;
   m_oldvel = *vel_v;
  
   m_speed = vel_v->length();

   m_num_flockmates_seen       = 0;
   m_nearest_flockmate         = NULL;
   m_dist_to_nearest_flockmate = INFINITY;

   m_num_enemies_seen          = 0;
   m_nearest_enemy             = NULL;
   m_dist_to_nearest_enemy     = INFINITY;

   m_next = m_prev = NULL;

}  

// Destructor.
// Destroys indicated boid.

CBoid::~CBoid (void)
{

}

//////////////////////////
// public flocking methods
//////////////////////////

// FlockIt.
// Used for frame-by-frame updates; no time deltas on positions.

void CBoid::FlockIt (int flock_id, CBoid *first_boid)
{

   ofVec3f acc;

   // Step 1:  Update our position.
   // Update our position based on the velocity
   // vector we computed last time around.

   m_oldpos = m_pos;     // save off our previous position

   m_pos += m_vel;      // apply velocities.

   // Step 2:  SeeFriends.
   // Determine if we can see any of our flockmates.

   SeeFriends (first_boid);

   // Step 3:  Flocking behavior.
   // Do we see any of our flockmates?  If yes, it's time to implement
   // the first Three Rules (they don't matter if we can't see anybody)

   if (m_num_flockmates_seen) {

      // Step 4:  Implement Rule #1 (Separation).
      // Try to maintain our desired separation distance from our nearest flockmate.

      AccumulateChanges (acc, KeepDistance());

      // Step 5:  Implement Rule #2 (Alignment).
      // Try to move the same way our nearest flockmate does.

      AccumulateChanges (acc, MatchHeading());

      // Step 6:  Implement Rule #3 (Cohesion).
      // Try to move towards the center of the flock.

      AccumulateChanges (acc, SteerToCenter());
   
   }

   // Step 6:  The Fourth Rule (enemies)
   // If we're supposed to react to enemy flocks, determine
   // if there are any then avoid them if we can.

   if (REACT_TO_ENEMIES) {
      SeeEnemies (flock_id);
      AccumulateChanges (acc, FleeEnemies());
   }

   // Step 7:  Cruising.
   // Flockmates or not, enemies or not, figure out
   // how fast we'd move if it were all up to us.

   AccumulateChanges (acc, Cruising());

   // Step 8:  Constrain acceleration
   // If our acceleration change is more than we allow, constrain it

   if (acc.length() > MAX_CHANGE) {

      // definitely too much...constrain to maximum change

      acc.limit (MAX_CHANGE);

   }

   // Step 9:  Implementation.
   // Here's where we apply our newly computed acceleration vector
   // to create a new velocity vector to use next update cycle.

   m_oldvel = m_vel;     // save off our previous velocity

   // now add in the acceleration

   m_vel += acc;

   // Step 10:  constraint Y velocity changes.
   // Attempt to restrict flight straight up/down by damping out Y axis velocity.
   // This isn't strictly necessary, but does lead to more realistic looking flight.

   m_vel.y *= MAX_URGENCY;

   // Step 11:  Constrain our speed.
   // If we're moving faster than we're allowed to move, constrain our velocity.

   if ((m_speed = m_vel.length()) > MAX_SPEED) {

      // definitely too fast...constrain to maximum speed

      m_vel.limit (MAX_SPEED);

      m_speed = MAX_SPEED;

   }

   // Step 12:  Compute roll/pitch/yaw.
   // Compute our orientation after all this speed adjustment nonsense.

   ComputeRPY();

   // Step 13:  World boundaries.
   // If we've wandered outside the world bounds put us back in.  You might
   // not need to do this for your world, but that depends on your implementation.

   WorldBound();

}

///////////////////////////
// private flocking methods
///////////////////////////

// Cruising.
// Generates a vector indicating how a flock boid would
// like to move, if it were all up to him and he was under
// no other influences of any kind.

ofVec3f CBoid::Cruising (void)
{
   ofVec3f  change = m_vel;

   float  diff = (m_speed - DESIRED_SPEED)/ MAX_SPEED;
   float  urgency = (float) fabs(diff);

   // constrain the urgency level

   if (urgency < MIN_URGENCY) urgency = MIN_URGENCY;
   if (urgency > MAX_URGENCY) urgency = MAX_URGENCY;

   // Now add some "jitter" so that each boid has a 
   // bit of randomness just to keep things interesting.
   // This will also get us moving if we happen to start
   // things standing perfectly still (which is sorta boring).

   float jitter = ofRandomuf();

   if (jitter < 0.45f) {
      change.x += MIN_URGENCY * (diff < 0 ? -1 : 1);
   } else if (jitter < 0.90f) {
      change.z += MIN_URGENCY * (diff < 0 ? -1 : 1);
   } else {
      change.y += MIN_URGENCY * (diff < 0 ? -1 : 1);   // we don't like vertical motion all that much
   }

   // compute velocity change necessary to get to our desired cruising speed

   change.scale ((urgency * (diff > 0 ? -1 : 1) ));

   return (change);
}

// FleeEnemies.
// Generates a vector for a flock boid to avoid the 
// nearest enemy (boid of a different flock) it sees.

ofVec3f CBoid::FleeEnemies (void)
{
   ofVec3f change;

   // test:  Are we too close to our nearest enemy?

   if (m_dist_to_nearest_enemy < KEEP_AWAY_DIST) {

      // yep...compute vector away from enemy

      change = m_pos - m_nearest_enemy->m_pos;

   }

   // return change vector

   return (change);

}

// KeepDistance.
// Generates a vector for a flock boid to maintain his
// desired separation distance from the nearest flockmate he sees.

ofVec3f CBoid::KeepDistance (void)
{

   float ratio = m_dist_to_nearest_flockmate/SEPARATION_DIST;

   // compute vector towards our nearest buddy

   ofVec3f change = m_nearest_flockmate->m_pos - m_pos;   

   // constrain computed ratio to our min/max Urgency levels

   if (ratio < MIN_URGENCY) ratio = MIN_URGENCY;
   if (ratio > MAX_URGENCY) ratio = MAX_URGENCY;

   // test:  are we too close to our nearest flockmate?

   if (m_dist_to_nearest_flockmate < SEPARATION_DIST) {

      // too close...move away from our neighbor

      change.scale (-ratio);

   } else if (m_dist_to_nearest_flockmate > SEPARATION_DIST) {

      // too far away...move towards our neighbor

      change.scale (ratio);

   } else {

      // in the UNLIKELY event we're exactly the right distance away, do nothing

      change.scale (0.0);
   }

   // return change vector

   return (change);
}

// MatchHeading.
// Generates a vector for a flock boid to try
// to match the heading of its nearest flockmate.

ofVec3f CBoid::MatchHeading (void)
{

   // copy the heading of our nearest buddy

   ofVec3f change = m_nearest_flockmate->m_vel;

   // normalize and then scale our vector change a bit...after
   // all we can't instantly snap to a new heading, can we?

   change.scale (MIN_URGENCY);

   // return change vector

   return (change);

}

// SeeEnemies.
// Determines which enemy flock boids a given flock boid can see.

int CBoid::SeeEnemies (int flock_id)
{

   float dist;

   CBoid *enemy;

   // initialize enemy data

   m_num_enemies_seen      = 0;
   m_nearest_enemy         = NULL;
   m_dist_to_nearest_enemy = INFINITY;

   // loop over each flock and determine the closest one we can see

   for (int i = 0; i < CFlock::FlockCount; i++) {

      // test:  if this is OUR flock, skip it

      if (i == flock_id) continue;

      // not our flock, so check it out

     CFlock * flk = CFlock::ListOfFlocks[i];
     
     if(flk)
       enemy = flk->GetFirstMember();
     else
       enemy = NULL;

      while (enemy != NULL) {
      
         // if this enemy is visible...

         if ((dist = CanISee(enemy)) != INFINITY) {

            // I can see it..increment counter

            m_num_enemies_seen++;

            // Test:  Closest enemy?

            if (dist < m_dist_to_nearest_enemy) {
               
               // yes...save it off

               m_dist_to_nearest_enemy = dist;
               m_nearest_enemy = enemy;
            }
         }

         // get next enemy in flock

         enemy = enemy->GetNext();

      }
   }

   return (m_num_enemies_seen);

}

// SeeFriends.
// Determines which flockmates a given flock boid can see.

int CBoid::SeeFriends (CBoid *first_boid)
{

   float dist;

   CBoid *flockmate = first_boid;

   // clear the existing visibility list of any holdover from last round

   ClearVisibleList();

   // now figure out who we can see

   while (flockmate != NULL) {

      // Test:  Within sight of this boid?

      if ((dist = CanISee(flockmate)) != INFINITY) {

         // add it to the list

         AddToVisibleList(flockmate);

         // Test:  If this guy is closer than the current
         // closest, make him the current closest

         if (dist < m_dist_to_nearest_flockmate) {
            m_dist_to_nearest_flockmate = dist;
            m_nearest_flockmate = flockmate;
         }
      }

      // next flockmate

      flockmate = flockmate->GetNext();
   }

   return (m_num_flockmates_seen);
}

// SteerToCenter.
// Generates a vector to guide a flock boid towards
// the "center of mass" of the flockmates he can see.

ofVec3f CBoid::SteerToCenter (void)
{

   ofVec3f center, change;

   // walk down the visibility list and sum up their position vectors

   for (int i = 0; i < m_num_flockmates_seen; i++) {
      if (VisibleFriendsList[i] != NULL) center += VisibleFriendsList[i]->m_pos;
   }

   // average the positions to get the perceived center of the flock

   center /= m_num_flockmates_seen;

   // now that we have the perceived center, computer a vector towards it

   change = center - m_pos;

   // Scale the change vector so we don't turn on a dime..

   change.scale (MIN_URGENCY);

   // return change vector

   return (change);
}

// WorldBound.
// Implements a world boundary so that flocks don't fly
// infinitely far away from the camera, instead remaining 
// in a nice viewable area.  It does this by wrapping flock
// boids around to the other side of the world, so (for example)
// they move out the right and return on the left.
// Returns:  nothing.

void CBoid::WorldBound (void)
{

   float maxX = CBox::WorldPtr->GetBoxWidth()/2;
   float maxY = CBox::WorldPtr->GetBoxHeight()/2;
   float maxZ = CBox::WorldPtr->GetBoxLength()/2;

   float minX = -maxX;
   float minY = -maxY;
   float minZ = -maxZ;

   // test position of flock boid and 
   // 'warp' if they've strayed out of bounds

   if (m_pos.x > maxX) {
      m_pos.x = minX;
   } else if (m_pos.x < minX) {
      m_pos.x = maxX;
   
   } else if (m_pos.y > maxY) {
      m_pos.y = minY;
   } else if (m_pos.y < minY) {
      m_pos.y = maxY;
   
   } else if (m_pos.z > maxZ) {
      m_pos.z = minZ;
   } else if (m_pos.z < minZ) {
      m_pos.z = maxZ;
   }

}

////////////////////////
// miscellaneous methods
////////////////////////

// AccumulateChanges.
// Adds vector values in changes into the accumumlator vector.
// Returns:  magnitude of accumulator vector after adding changes.

float CBoid::AccumulateChanges (ofVec3f &accumulator, 
                                ofVec3f changes)
{

   // apply the change

   accumulator += changes;

   return (accumulator.length());

}

// AddToVisibleList.
// Adds a boid to the list of visible boids.
// This visibility list is regenerated for each boid each update cycle,
// and acts much like a push-down queue; the latest boid added to the
// list becomes the first one when the list is sequentially accessed.
// Mostly I did this for speed reasons, as this allows for fast inserts
// (and we don't delete from this list, we just rebuild it each update cycle).

void CBoid::AddToVisibleList (CBoid *ptr)
{

   // test:  do we see enough buddies already?

   if (m_num_flockmates_seen < MAX_FRIENDS_VISIBLE) {

      // nope--we can add to this one to the list

      VisibleFriendsList[m_num_flockmates_seen] = ptr;

      // increment counter

      m_num_flockmates_seen++;

   }

}

// ClearVisibleList.
// Clears the visibility list.

void CBoid::ClearVisibleList (void)
{

   // walk down the list and clear each visible entry

   for (int i = 0; i < MAX_FRIENDS_VISIBLE; i++) {
      VisibleFriendsList[i] = NULL;
   }

   // clear other visibility info

   m_num_flockmates_seen       = 0;
   m_nearest_flockmate         = NULL;
   m_dist_to_nearest_flockmate = INFINITY;

}

// Determine whether a given invoking boid can see the boid in question.
// Returns the distance to the boid.

float CBoid::CanISee (CBoid *ptr)
{

   // Test:  if we're looking at ourselves, never mind

   if (this == ptr) return (INFINITY);

   // figure out distance

   float dist = (m_pos.distance(ptr->m_pos));

   // if we're using truth data, don't bother to check sighting

   if (USE_TRUTH) return (dist);

   // not using truth, so check sighting

   if (m_perception_range > dist) return (dist);

   // fell through; can't see it

   return (INFINITY);

}

// ComputeRPY.
// Computes the roll/pitch/yaw of the flock boid based on its
// latest velocity vector changes.  Roll/pitch/yaw are stored in
// the "ang" data boid as follows:
//   pitch is about the x axis
//   yaw is about the y axis
//   roll is about the z axis
//
// All calculations assume a right-handed coordinate system:
//   +x = through the left side of the object
//   +y = up
//   +z = through the nose of the model
//
// All angles are computed in radians.
//
// NOTE:  This algorithm was generously provided by Christopher Kline, who originally
//        developed it for *his* flocking applications.  Thanks Christopher!

void CBoid::ComputeRPY (void)
{

   float roll, pitch, yaw;

   // Determine the direction of the lateral acceleration.

   ofVec3f lateralDir = (m_vel.crossed(m_vel - m_oldvel)).crossed(m_vel);

   lateralDir.normalize();

   // Set the lateral acceleration's magnitude. The magnitude is the vector
   // projection of the appliedAcceleration vector onto the direction of the
   // lateral acceleration).
  
   float lateralMag = (m_vel - m_oldvel).dot(lateralDir);
  
   // compute roll

   if (lateralMag == 0) {
      roll = 0.0f;  
   } else {
      roll = (float) -atan2(GRAVITY, lateralMag) + HALF_PI;
   }

   // compute pitch

   pitch = (float) -atan(m_vel.y / sqrt((m_vel.z*m_vel.z) + (m_vel.x*m_vel.x)));

   // compute yaw

   yaw = (float) atan2(m_vel.x, m_vel.z);

   // store them

   m_ang.x = pitch;

   m_ang.y = yaw;

   m_ang.z = roll;

}

// SetNext.
// Set the "next" pointer of an individual boid.

void CBoid::SetNext (CBoid *ptr)
{

   m_next = ptr;

}

// GetNext.
// Returns the "next" pointer of the invoking boid.

CBoid * CBoid::GetNext()
{

   return (m_next);

}

// SetPrev.
// Set the "prev" pointer of an individual boid.

void CBoid::SetPrev (CBoid *ptr)
{

   m_prev = ptr;

}

// GetOrient.
// Returns the orientation of the boid in question.

ofVec3f * CBoid::GetOrient (void)
{

   return (&m_ang);

}

// GetPos.
// Returns the position of the boid in question.

ofVec3f * CBoid::GetPos (void)
{

   return (&m_pos);

}

// LinkOut.
// Removes a boid from a list.

void CBoid::LinkOut ()
{

   // test location of boid

   if ((m_next == NULL) && (m_prev == NULL)) {

      SetNext(NULL);
      SetPrev(NULL);

   } else if (m_next == NULL) {

      m_prev->SetNext(NULL);

   } else if (m_prev == NULL) {
      
      m_next->SetPrev(NULL);

   } else {

      m_prev->SetNext(m_next);
      m_next->SetPrev(m_prev);
   }
}
