
#include "NavigationBase.h"
#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/User.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

namespace Navigation
{

  NavigationBase::NavigationBase (vrj::App *pApp)
          :mApp             	(pApp    ),
           mNavigationMatrix	(		   ),
           mUserName        	("User1"),
           //mUserHeadProxy   	(NULL    ),
           mRotateFactor    	(1.0f    ),
           mTranslateFactor 	(1.0f    )
           //mLastUpdate	 	(	0	 )
  {
  }

  NavigationBase::NavigationBase()
  {
    //NavigationBase(0);
  }
  NavigationBase::~NavigationBase ()
  {

  }

  void
  NavigationBase::initialize(){

      vrj::UserPtr pUser = vrj::Kernel::instance()->getUser(mUserName);

      std::cout << "user ---------- > " << pUser << std::endl;
      if(pUser!= NULL)
      {
          mUserHeadProxy = pUser->getHeadPosProxy();

          if(mUserHeadProxy == NULL)
          {
              vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                      << "NavigationBase: Failed to initalize user head proxy."
                      << std::endl << vprDEBUG_FLUSH;
          }
      }
      else
      {
          vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                  << "NavigationBase:Failed to initalize user named ["
                  << mUserName << "]."
                  << std::endl << vprDEBUG_FLUSH;
      }

      gmtl::identity(mNavigationMatrix);

  }


  vrj::App*
  NavigationBase::getApp(){
    return mApp;
  }

  float
  NavigationBase::getRotationFactor(){
    return mRotateFactor;
  }

  float
  NavigationBase::getTransulationFactor(){
    return mTranslateFactor;
  }

  gmtl::Matrix44f*
  NavigationBase::getNavigationMatrix(){
    return &mNavigationMatrix;
  }

  void
  NavigationBase::navigate(gmtl::Matrix44f& matrix){
    vrj::UserPtr pUser = vrj::Kernel::instance()->getUser(mUserName);
    if(pUser != NULL)
    {
      mUserHeadProxy = pUser->getHeadPosProxy();
      if(mUserHeadProxy == NULL)
      {
        vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                    << "NavigationBase: Failed to initalize user head proxy."
                        << std::endl << vprDEBUG_FLUSH;
       }
    }
    else
    {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
          << "NavigationBase:Failed to initalize user named ["
          << mUserName << "]."
          << std::endl << vprDEBUG_FLUSH;
    }
    if(mLastUpdate.secf() == 0.0){
      mFPSFactor=0.005;
      mLastUpdate=mUserHeadProxy->getTimeStamp();
    }else{
      vpr::Interval now = mUserHeadProxy->getTimeStamp();
      mFPSFactor		  = (now-mLastUpdate).secf();
      mLastUpdate		  = now;
    }
    mTranslateFactor=100*mFPSFactor;
    mRotateFactor	=0.5*mFPSFactor;
    gmtl::preMult(matrix,mNavigationMatrix);
    gmtl::identity(mNavigationMatrix);
  }

  void
  NavigationBase::moveForward(){
    gmtl::Vec3f z_dir ( 0.0f, 0.0f, 1.0f );
    gmtl::preMult (mNavigationMatrix, gmtl::makeTrans<gmtl::Matrix44f> (mTranslateFactor*z_dir ) );
  }

  void
  NavigationBase::moveBackward(){
    gmtl::Vec3f z_dir ( 0.0f, 0.0f, 1.0f );
    gmtl::preMult (mNavigationMatrix, gmtl::makeTrans<gmtl::Matrix44f> (-mTranslateFactor*z_dir ) );
  }

  void
  NavigationBase::turnLeft(){
    gmtl::preMult ( mNavigationMatrix,
                  gmtl::makeRot<gmtl::Matrix44f> (gmtl::EulerAngleXYZf ( 0.0f,-mRotateFactor, 0.0f ) )
                 );
  }

  void
  NavigationBase::turnRight(){
    gmtl::preMult ( mNavigationMatrix,
                    gmtl::makeRot<gmtl::Matrix44f> (gmtl::EulerAngleXYZf ( 0.0f,mRotateFactor, 0.0f ) )
                   );
  }

  void
  NavigationBase::lookUp(){
    gmtl::preMult ( mNavigationMatrix,
                  gmtl::makeRot<gmtl::Matrix44f> (gmtl::EulerAngleXYZf ( mRotateFactor,0.0f, 0.0f ) )
                 );
  }

  void
  NavigationBase::lookDown(){
    gmtl::preMult ( mNavigationMatrix,
                  gmtl::makeRot<gmtl::Matrix44f> (gmtl::EulerAngleXYZf ( -mRotateFactor,0.0f, 0.0f ) )
                 );
  }

  void
  NavigationBase::moveUp(){
    gmtl::Vec3f y_dir ( 0.0f, 1.0f, 0.0f );
    gmtl::preMult (mNavigationMatrix, gmtl::makeTrans<gmtl::Matrix44f> (mTranslateFactor*y_dir ) );
  }

  void
  NavigationBase::moveDown(){
    gmtl::Vec3f y_dir ( 0.0f, 1.0f, 0.0f );
    gmtl::preMult (mNavigationMatrix, gmtl::makeTrans<gmtl::Matrix44f> (-mTranslateFactor*y_dir ) );
  }

  void
  NavigationBase::exit(){
    vrj::Kernel::instance()->stop();
  }
}
