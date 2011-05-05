//Copyright (C) Brian Walsh, 2011
//Portions Copyright (C) Steven Woodcock, 2000
//*********************************************************************
// Name:     CBox.cpp: 
// Purpose:  Class methods for the CBox class.
//*********************************************************************

//
// includes
//

#include "CBox.h"
#include "ofGraphics.h"
#include "ofAppGlutWindow.h"

//
// flocking debug globals
//

extern bool gDrawAxes;

//
// static variable initialization
//

CBox * CBox::WorldPtr = NULL;

const GLfloat CBox::axes[] = {0.0, 0.0, 0.0,
                              5.0, 0.0, 0.0,
                              0.0, 5.0, 0.0,
                              0.0, 0.0, 5.0};
const GLfloat CBox::axesColor[] = {1.0, 1.0, 1.0, 1.0,
                                   1.0, 0.0, 0.0, 1.0,
                                   0.0, 1.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0, 1.0};

const GLubyte CBox::indices[] = {0,1,0,2,0,3};

const GLubyte CBox::boxIndices[] = {0,1,
                                    0,4,
                                    1,5,
                                    4,5,
                                    0,2,
                                    1,3,
                                    4,6,
                                    5,7,
                                    2,3,
                                    2,6,
                                    3,7,
                                    6,7};
///////////////////////////
// Construction/Destruction
///////////////////////////

CBox::CBox (float lv, float wv, float hv)
{

#ifdef BOX_DEBUG
   myprintf("\nInside Box Contructor\n");
   myprintf("Length = %f Width = %f Height = %f\n",lv, wv, hv);
#endif

   m_Length = lv;
   m_Width  = wv;
   m_Height = hv;

  boxVerts[0]=-m_Height/2.0,boxVerts[1]=-m_Height/2.0,boxVerts[2]=-m_Height/2.0;
  boxVerts[3]=-m_Height/2.0,boxVerts[4]=-m_Height/2.0,boxVerts[5]=m_Height/2.0;
  boxVerts[6]=-m_Height/2.0,boxVerts[7]=m_Height/2.0,boxVerts[8]=-m_Height/2.0;
  boxVerts[9]=-m_Height/2.0,boxVerts[10]=m_Height/2.0,boxVerts[11]=m_Height/2.0;
  boxVerts[12]=m_Height/2.0,boxVerts[13]=-m_Height/2.0,boxVerts[14]=-m_Height/2.0;
  boxVerts[15]=m_Height/2.0,boxVerts[16]=-m_Height/2.0,boxVerts[17]=m_Height/2.0;
  boxVerts[18]=m_Height/2.0,boxVerts[19]=m_Height/2.0,boxVerts[20]=-m_Height/2.0;
  boxVerts[21]=m_Height/2.0,boxVerts[22]=m_Height/2.0,boxVerts[23]=m_Height/2.0;

  WorldPtr = this;

}

CBox::~CBox ()
{

#ifdef BOX_DEBUG
   myprintf("\nInside CBox Destructor\n");
#endif

}

////////////////////
// rendering methods
////////////////////

void CBox::Draw ()
{

   ofPushMatrix();

      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  
      glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, boxVerts);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, boxIndices);
      glDisableClientState(GL_VERTEX_ARRAY);
  
      // draw world XYZ axes?

//      if (gDrawAxes) {

//      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//      
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, axes);
        glColorPointer(4, GL_FLOAT, 0, axesColor);
  
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, indices);

      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);

//      }

   ofPopMatrix();

}

//////////////////////////
// miscellaneous functions
//////////////////////////

// GetBoxLength.
// Returns the length of the Box, in meters.

float CBox::GetBoxLength (void)
{ return (m_Length); }

// GetBoxWidth..
// Returns the width of the Box, in meters.

float CBox::GetBoxWidth (void)
{ return (m_Width); }

// GetBoxHeight.
// Returns the height of the Box, in meters.

float CBox::GetBoxHeight (void)
{ return (m_Height); }
