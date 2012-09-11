/*
 * NavigationBase.h
 *
 *  Created on: Dec 16, 2009
 *      Author: murali
 */

#ifndef NAVIGATIONBASE_H_
#define NAVIGATIONBASE_H_

#include <vrj/Kernel/App.h>
#include <vpr/Util/Interval.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/PositionProxy.h>
#include <gmtl/Matrix.h>

namespace Navigation
{

  class NavigationBase
  {
    public:
      NavigationBase (vrj::App *pApp);
      NavigationBase ();

      virtual ~NavigationBase ();


      void initialize();
      virtual void update()	=	0;
      void navigate(gmtl::Matrix44f& matrix);

    protected:
      /*
       * Navigation Funcutions
       */
      void moveForward();
      void moveBackward();
      void turnLeft();
      void turnRight();
      void lookUp();
      void lookDown();
      void moveUp();
      void moveDown();
      void exit();


      /*
       * Return Function
       */
    public:
      vrj::App* getApp();
      float getRotationFactor();
      float getTransulationFactor();
      gmtl::Matrix44f* getNavigationMatrix();
      vrj::App  *mApp;
      gmtl::Matrix44f mNavigationMatrix;
      float	mFPSFactor;
      float   mRotateFactor;
      float   mTranslateFactor;
    private:
      std::string            	mUserName;
      gadget::PositionProxyPtr mUserHeadProxy;
      vpr::Interval mLastUpdate;
  };

}

#endif /* NAVIGATIONBASE_H_ */
