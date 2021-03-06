/*
 * KeyBoardMouseNavigation.cpp
 *
 *  Created on: Dec 23, 2009
 *      Author: murali
 */
#include <vtkVRJugglerGUIHelper.h>
#include "KeyBoardMouseNavigation.h"
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>

namespace Navigation
{

  KeyBoardMouseNavigation::KeyBoardMouseNavigation ()
  {


  }

  KeyBoardMouseNavigation::KeyBoardMouseNavigation(vrj::App* pApp)
              :Inherited(pApp)
  {

  }
  KeyBoardMouseNavigation::~KeyBoardMouseNavigation ()
  {

  }

  void
  KeyBoardMouseNavigation::initialize()
  {
    Inherited::initialize();

    mKeyBoardMouse.init("VJKeyboard");

  }
  void
  KeyBoardMouseNavigation::update(){

    gadget::KeyboardMouse::EventQueue evt_queue = mKeyBoardMouse->getEventQueue ();

    gadget::KeyboardMouse::EventQueue::iterator itor = evt_queue.begin ();
    for(itor = evt_queue.begin(); itor!=evt_queue.end(); ++itor)
    {
      const gadget::EventType type = (*itor)->type ();
      if (type == gadget::KeyPressEvent)
      {
        gadget::KeyEventPtr key_evt = boost::dynamic_pointer_cast<gadget::KeyEvent>(*itor);
        processKeyBoardEvents(key_evt);
      }
      else if(type == gadget::MouseButtonPressEvent   ||
          type == gadget::MouseButtonReleaseEvent ||
          type == gadget::MouseMoveEvent)

      {
        gadget::MouseEventPtr mouse_evt = boost::dynamic_pointer_cast<gadget::MouseEvent>(*itor);
        processMouseEvents(mouse_evt);
      }
    }

  }

  void
  KeyBoardMouseNavigation::processKeyBoardEvents(gadget::KeyEventPtr pEvent){


  }

  void
  KeyBoardMouseNavigation::processMouseEvents(gadget::MouseEventPtr pEvent){
    double VRJ_PI = 3.1415927;
    static bool bLeftPressed  = false;
    static bool bMidPressed   = false;
    static bool bRightPressed = false;
    static gmtl::Vec3f v3LastPos;

    gmtl::Vec3f v3CurPos;
    convertMouseCoordinate2WorldCoordinate(pEvent,v3CurPos);
    gmtl::Matrix44f* Mat = getNavigationMatrix();

    if (bLeftPressed)
    {

      if ( (pEvent->getButton () ==  gadget::MBUTTON1) &&
         (pEvent->type      () ==  gadget::MouseButtonReleaseEvent))
      {
        bLeftPressed = false;
      }
      else
      {
        float fYaw   = 2 * VRJ_PI * (v3CurPos[0] - v3LastPos[0])*mFPSFactor*0.1;
        float fPitch = 2 * VRJ_PI * (v3CurPos[1] - v3LastPos[1])*mFPSFactor*0.1;

        gmtl::preMult(*Mat,
                gmtl::makeRot<gmtl::Matrix44f> ( gmtl::EulerAngleXYZf ( -fPitch*180.0,
                                             fYaw*180.0,
                                             0.0f ) ) );
        v3LastPos = v3CurPos;
      }

    }
    else if (bMidPressed)
    {

      if ((pEvent->getButton() == gadget::MBUTTON2) &&
        (pEvent->type 	  () == gadget::MouseButtonReleaseEvent))
      {
        bMidPressed = false;
      }
      else
      {
        gmtl::Vec3f fTemp = (v3CurPos - v3LastPos);
        fTemp[0] = fTemp[0] * (1/mFPSFactor);
        fTemp[1] = fTemp[1] * (1/mFPSFactor);
        fTemp[2] = fTemp[2] * (1/mFPSFactor);

        gmtl::preMult(*Mat,
                gmtl::makeTrans<gmtl::Matrix44f>(fTemp));

        v3LastPos = v3CurPos;

      }
    }
    else if (bRightPressed)
    {
      if ((pEvent->getButton () == gadget::MBUTTON3) && (pEvent->type ()
          == gadget::MouseButtonReleaseEvent))
      {
        bRightPressed = false;
      }
      else
      {
        gmtl::Vec3f v3TempPos;
        v3TempPos = v3CurPos - v3LastPos;
        gmtl::Vec3f v3OldTrans;
        v3OldTrans[0] = v3TempPos[0] * (1/mFPSFactor);
        v3OldTrans[1] = v3TempPos[2] * (1/mFPSFactor);
        v3OldTrans[2] = v3TempPos[1] * (1/mFPSFactor);

        gmtl::preMult(*Mat,
                gmtl::makeTrans<gmtl::Matrix44f>(v3OldTrans));

        v3LastPos = v3CurPos;
      }
    }
    else if ((pEvent->getButton () == gadget::MBUTTON1) &&
         (pEvent->type 		() == gadget::MouseButtonPressEvent))
    {
      bLeftPressed = true;
      v3LastPos = v3CurPos;
    }
    else if ((pEvent->getButton () == gadget::MBUTTON2) &&
         (pEvent->type 		() == gadget::MouseButtonPressEvent))
    {
      bMidPressed = true;
      v3LastPos = v3CurPos;
    }
    else if ((pEvent->getButton () == gadget::MBUTTON3) &&
         (pEvent->type 		() == gadget::MouseButtonPressEvent))
    {
      bRightPressed = true;
      v3LastPos = v3CurPos;
    }

  }



  void
  KeyBoardMouseNavigation::convertMouseCoordinate2WorldCoordinate( gadget::MouseEventPtr pEvent,
                                                                   gmtl::Vec3f& WorldPos )

  {


    std::vector<vrj::DisplayPtr> vecDisps = ((vtkVRJugglerGUIHelper*)getApp())->getDisplayManager()->getAllDisplays();

    if (vecDisps.size () <= 0)
      return;

    vrj::DisplayPtr pDisp = vecDisps[0];

    if (pDisp->getNumViewports () <= 0)
      return;

    // display pixel size
    int dispXOrigin, dispYOrigin, dispWidth, dispHeight;
    pDisp->getOriginAndSize ( dispXOrigin,dispYOrigin,dispWidth,dispHeight );

    // view frustum
    gmtl::Vec3f apex, ur, lr, ul, ll;
    vrj::ProjectionPtr pLeftProj = pDisp->getViewport ( 0 )->getLeftProj ();
    pLeftProj->getFrustumApexAndCorners ( apex,ur,lr,ul,ll );
    // current mouse position in viewport
    float mouseXPos = (float) pEvent->getX ();
    float mouseYPos = (float) pEvent->getY ();

    // bilinear interpolation
    float xIpol = mouseXPos / dispWidth;
    float yIpol = mouseYPos / dispHeight;

    gmtl::Vec3f upPos = ul + (ur - ul) * xIpol;
    gmtl::Vec3f lowPos = ll + (lr - ll) * xIpol;
    WorldPos = upPos + (lowPos - upPos) * yIpol;

  }
}
