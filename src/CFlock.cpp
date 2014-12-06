//Copyright (C) Brian Walsh, 2011
//Portions Copyright (C) Steven Woodcock, 2000
//*********************************************************************
// Name:     CFlock.cpp
// Purpose:  Class definitions and method prototypes for the flocks
//           themselves.
//*********************************************************************

//
// includes
//

#include "CFlock.h" 
#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofVectorMath.h"
#include "ofAppGlutWindow.h"

//
// flocking debug globals
//

extern bool gDrawAxes;
extern bool gDrawPerceptionDist;
extern bool gDrawKeepawayDist;
extern bool gDrawSeparationDist;

//
// static variable initialization
//

int CFlock::FlockCount = 0;
CFlock * CFlock::ListOfFlocks[] = {NULL};

const GLfloat CFlock::boidVert[] = {0.0, 0.0, 0.0,
                                    -2.5, 0.0, 0.0,
                                    2.5, 0.0, 0.0,
                                    0.0, -2.5, 0.0,
                                    0.0, 2.5, 0.0,
                                    0.0, 0.0, 10.0};

const GLubyte CFlock::boidIndices[] = {4,1,0,
                                       3,1,0,
                                       4,2,0,
                                       3,2,0,
                                       4,5,2,
                                       4,5,1,
                                       3,5,2,
                                       3,5,1};

const GLfloat CFlock::axes[] = {0.0, 0.0, 0.0,
                                2.0, 0.0, 0.0,
                                0.0, 2.0, 0.0,
                                0.0, 0.0, 2.0};
const GLfloat CFlock::axesColor[] = {1.0, 1.0, 1.0, 1.0,
                                     1.0, 0.0, 0.0, 1.0,
                                     0.0, 1.0, 0.0, 1.0,
                                     0.0, 0.0, 1.0, 1.0};

const GLubyte CFlock::axesIndices[] = {0,1,0,2,0,3};

//
// constructor and destructor methods
//

// Constructor.
// Creates a new flock.

CFlock::CFlock (int boidCount)
{

   // initialize internals

   m_id           = FlockCount;

   m_num_members  = 0;

   m_first_member = NULL;

   // increment counter
   ListOfFlocks[FlockCount] = this;
  
   FlockCount++;

  for(int i=0; i<boidCount; i++)
    AddTo(new CBoid(i));
  
   return;

}

// Destructor.

CFlock::~CFlock (void)
{

   // clear values

   m_id = m_num_members = 0;

   m_first_member = NULL;

   // decrement counter

   FlockCount--;

   return;

}

/////////////////////
// flocking functions
/////////////////////

// Update.
// Updates all members of a flock.

void CFlock::Update (void)
{

   CBoid *ptr;

   // loop over all members of this flock, invoke
   // their flocking method, then draw them

   ptr = m_first_member;

   while (ptr != NULL) {

      // flock the boid

      ptr->FlockIt(m_id,m_first_member);

      // get next boid

      ptr = ptr->GetNext();
   }
}

//////////////////////
// rendering functions
//////////////////////

void CFlock::Draw ()
{

   CBoid *ptr;

   ofVec3f *pos, *orient;

   float color_r, color_g, color_b;

   // select this flock's color

   switch (m_id) 
   {
      case 0:                             // red 
         color_r = 1.0f;
         color_g = 0.1f;
         color_b = 0.0f;
         break;
      case 1:                             // orange
         color_r = 1.0f;
         color_g = 0.5f;
         color_b = 0.0f;
         break;
      case 2:                             // magenta
         color_r = 1.0f;
         color_g = 0.0f;
         color_b = 1.0f;
         break;
      case 3:                             // blue 
         color_r = 0.0f;
         color_g = 0.0f;
         color_b = 1.0f;
         break;
      case 4:                             // green 
         color_r = 0.0f;
         color_g = 1.0f;
         color_b = 0.0f;
         break;
      default:                             // black 
         color_r = 0.0f;
         color_g = 0.0f;
         color_b = 0.0f;
         break;
   }

   // loop over all members of this flock, extract
   // their position and orientation, and then draw them

   ptr = m_first_member;

   while (ptr != NULL) {

      pos = ptr->GetPos();
      orient = ptr->GetOrient();

      // push our world matrix down

      ofPushMatrix();

         // translate to where this boid is
         glTranslatef(pos->x, pos->y, pos->z);

         // rotate to the boid's frame

         glRotatef(RAD_TO_DEG * orient->z, 0.0, 0.0, 1.0);   // roll
         glRotatef(RAD_TO_DEG * orient->y, 0.0, 1.0, 0.0);   // yaw
         glRotatef(RAD_TO_DEG * orient->x, 1.0, 0.0, 0.0);   // pitch

         // test for any debug options

         // draw perception distance spheres?

         //if (gDrawPerceptionDist) {
//            glColor3f (1.0f, 1.0f, 1.0f);
//            glutWireSphere (DEFAULT_PERCEPTION_RANGE, 12, 12);
         //}

         // draw keepaway distance spheres?

         //if (gDrawKeepawayDist) {
//            glColor3f (1.0f, 0.0f, 0.0f);
//            glutWireSphere (KEEP_AWAY_DIST, 12, 12);
         //}

         // draw separation distance spheres?

         //if (gDrawSeparationDist) {
//            glColor3f (color_r, color_g, color_b);
//            glutWireSphere (SEPARATION_DIST, 12, 12);
        // }

         // draw local (boid space) XYZ axes?

         //if (gDrawAxes) {

         
         glEnableClientState(GL_VERTEX_ARRAY);
         glEnableClientState(GL_COLOR_ARRAY);
         
           glVertexPointer(3, GL_FLOAT, 0, axes);
           glColorPointer(4, GL_FLOAT, 0, axesColor);
           
           glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, axesIndices);
         
         glDisableClientState(GL_VERTEX_ARRAY);
         glDisableClientState(GL_COLOR_ARRAY);
         //}

         // set this boid's color

         glColor4f(color_r, color_g, color_b, 1.0);

         // draw the boid

         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_FLOAT, 0, boidVert);
         glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, boidIndices);
         glDisableClientState(GL_VERTEX_ARRAY);

            // make tops slightly darker

         glColor4f(color_r - 0.1f, color_g - 0.1f, color_b - 0.1f, 1.0f);

         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_FLOAT, 0, boidVert);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boidIndices+12);
         glDisableClientState(GL_VERTEX_ARRAY);

            // make bottoms slightly lighter

         glColor4f(color_r + 0.1f, color_g + 0.1f, color_b + 0.1f, 1.0f);

         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_FLOAT, 0, boidVert);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, boidIndices+18);
         glDisableClientState(GL_VERTEX_ARRAY);

         // Pop the rotation matrix from the MODELVIEW stack

         ofPopMatrix();

      // get next boid

      ptr = ptr->GetNext();
   }
}

//////////////////////////
// miscellaneous functions
//////////////////////////

// AddTo.
// Adds the indicated member to the flock.

void CFlock::AddTo (CBoid * member)
{

   CBoid *ptr;

   // if this is the first member added to this
   // flock, store him in the first_member slot

   if (!m_first_member) {
      
      m_first_member = member;

      member->SetNext(NULL);
      member->SetPrev(NULL);

   } else {
   
      // not the first member...follow the list
      // of members and add this guy to the end

      ptr = m_first_member;

      while (ptr->GetNext() != NULL) ptr = ptr->GetNext();
         
      // we should now be at the end...add our new guy

      ptr->SetNext(member);
      member->SetPrev(ptr);

   }

   // increment # of members in this flock

   m_num_members++;

}

// GetCount.
// Returns the # of members in a given flock.

int CFlock::GetCount (void)
{

   return (m_num_members);

}

// GetFirstMember.
// Returns a pointer to the first member of a given flock (if any).

CBoid * CFlock::GetFirstMember (void)
{

   // return pointer to first member of flock

   return (m_first_member);

}

// RemoveFrom.
// Removes the indicated member from the flock.

void CFlock::RemoveFrom (CBoid * member)
{

   CBoid *ptr = m_first_member;

   // search for the indicated member

   while (ptr != NULL)
   {

      // test:  find it yet?

      if (ptr == member) break;

      ptr = ptr->GetNext();

   }

   // if we found it, remove it from the list

   if (ptr) {

      // found it...see if we have to tweak first member

      if (ptr == m_first_member) {

         if (m_num_members == 1) {
            m_first_member = NULL;
         } else {
            m_first_member = ptr->GetNext();
         }
      }

      // now delink it

      ptr->LinkOut();

      // cleanup

      ptr->SetNext(NULL);
      ptr->SetPrev(NULL);

      // decrement counter

      m_num_members--;

   } else {

   }

}