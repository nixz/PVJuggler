//-----------------------------------------------------------------------------
/**
 * @file   vtkVRJNavigation.cpp
 * @author Nix <nikhil@naunine.com>
 * @date   Mon Mar  1 13:36:14 2010
 *
 * @brief
 */
//-----------------------------------------------------------------------------

#include "vtkVRJNavigation.h"
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

vtkVRJNavigation::vtkVRJNavigation(bool t_or_r)
{
  _t_or_r = t_or_r;
  if(t_or_r==ROTATION)
    {
    setAzimuth("GamepadAnalog03Proxy");
    setElevation("GamepadAnalog04Proxy");
    setRoll("GamepadAnalog05Proxy");
    }
  if(t_or_r==TRANSLATION)
    {
    setForwardBackward("GamepadAnalog02Proxy");
    setLeftRight("GamepadAnalog01Proxy");
    setTopBottom("GamepadAnalog06Proxy");
    setButtons("GamepadButton01Proxy",
               "GamepadButton02Proxy",
               "GamepadButton03Proxy",
               "GamepadButton04Proxy",
               "GamepadButton05Proxy",
               "GamepadButton06Proxy",
               "GamepadButton07Proxy",
               "GamepadButton08Proxy",
               "GamepadButton09Proxy",
               "GamepadButton10Proxy",
               "GamepadButton11Proxy",
               "GamepadButton12Proxy");

    }

//  setTranslateScale("GamepadAnalog06Proxy");
//  setRotateScale("GamepadAnalog05Proxy");

  // setYaw("GamepadAnalog03Proxy");
  // setPitch("GamepadAnalog02Proxy");




  _txScale=100.0;
  _rxScale=.1;
  gmtl::identity(_navMatrix);
}

vtkVRJNavigation::~vtkVRJNavigation()
{
}

void vtkVRJNavigation::setForwardBackward(const std::string name)
{
  _forwardBack.init(name);
}

void vtkVRJNavigation::setLeftRight(const std::string name)
{
  _leftRight.init(name);
}

void vtkVRJNavigation::setTopBottom(const std::string name)
{
  _topBottom.init(name);
}

void vtkVRJNavigation::setYaw(const std::string name)
{
  _yaw.init(name);
}

void vtkVRJNavigation::setPitch(const std::string name)
{
  _pitch.init(name);
}

void vtkVRJNavigation::setAzimuth(const std::string name)
{
  _azimuth.init(name);
}

void vtkVRJNavigation::setElevation(const std::string name)
{
  _elevation.init(name);
}

void vtkVRJNavigation::setRoll(const std::string name)
{
  _roll.init(name);
}

void vtkVRJNavigation::setButtons(const std::string button01,
                                  const std::string button02,
                                  const std::string button03,
                                  const std::string button04,
                                  const std::string button05,
                                  const std::string button06,
                                  const std::string button07,
                                  const std::string button08,
                                  const std::string button09,
                                  const std::string button10,
                                  const std::string button11,
                                  const std::string button12)
{
  _b[0].init(button01);
  _b[1].init(button02);
  _b[2].init(button03);
  _b[3].init(button04);
  _b[4].init(button05);
  _b[5].init(button06);
  _b[6].init(button07);
  _b[7].init(button08);
  _b[8].init(button09);
  _b[9].init(button10);
  _b[10].init(button11);
  _b[11].init(button12);
}

void vtkVRJNavigation::update()
{
  gmtl::identity(_navMatrix);
  if(_t_or_r==ROTATION)
    {
    updateAzimuth();
    updateElevation();
    updateRoll();
    }
  if(_t_or_r==TRANSLATION)
    {
    updateForwardBack();
    updateLeftRight();
    updateTopBottom();
    updateButtons();
    }
  // updateYaw();
  // updatePitch();




}

void vtkVRJNavigation::updateButtons()
{
  float b1 = _b[0]->getData();
  float b2 = _b[1]->getData();
  float b3 = _b[2]->getData();
  float b4 = _b[3]->getData();
  float b5 = _b[4]->getData();
  float b6 = _b[5]->getData();
  float b7 = _b[6]->getData();
  float b8 = _b[7]->getData();
  float b9 = _b[8]->getData();
  float b10 = _b[9]->getData();
  float b11 = _b[10]->getData();
  float b12 = _b[11]->getData();

printf("b1:%3.3f b2:%3.3f \n b3:%3.3f b4:%3.3f \n \
b5:%3.3f b6:%3.3f \n b7:%3.3f b8:%3.3f \n b9:%3.3f b10:%3.3f \n\
b11:%3.3f b12:%3.3f\n",b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12);
}

void vtkVRJNavigation::updateForwardBack()
{
  gmtl::Vec3f dir ( 0.0f, 0.0f, 1.0f );
  float forwardBack;
  if(_forwardBack->getData() <= 0.0f && _forwardBack->getData() >= 1.0f)
    forwardBack=0.0;
  else
    forwardBack = ((2*_forwardBack->getData())-1)*_txScale;

  vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
    <<"[vtkVRJNavigation::updateForwardBack]" << "forward backward "<< forwardBack
    <<vprDEBUG_FLUSH;

  gmtl::preMult
    (_navMatrix,
     gmtl::makeTrans<gmtl::Matrix44f> (forwardBack*dir));
}

void vtkVRJNavigation::updateLeftRight()
{
  gmtl::Vec3f dir ( 1.0f, 0.0f, 0.0f );
  float leftRight;
  if(_leftRight->getData() <= 0.0f && _leftRight->getData() >= 1.0)
    leftRight =0.0;
  else
    leftRight = ((2*_leftRight->getData())-1)*_txScale;

  vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
    <<"[vtkVRJNavigation::updateLeftRight]" << "left right "<< leftRight
    <<vprDEBUG_FLUSH;

  gmtl::preMult
    (_navMatrix,
     gmtl::makeTrans<gmtl::Matrix44f> (leftRight*dir));
}

void vtkVRJNavigation::updateTopBottom()
{
  gmtl::Vec3f dir (0.0f,1.0f,0.0f);
  float topBottom;
  if(_topBottom->getData() <= 0.0 && _topBottom->getData() >= 1.0)
    topBottom=0.0;
  else
    topBottom = ((2*_topBottom->getData())-1)*_txScale;

  vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
    <<"[vtkVRJNavigation::updateTopBottom]" << "bottom top "<< topBottom
    <<vprDEBUG_FLUSH;

  gmtl::preMult
    (_navMatrix,
     gmtl::makeTrans<gmtl::Matrix44f> (topBottom*dir));

}

void vtkVRJNavigation::updateAzimuth()
{
  float azimuth;
  if(_azimuth->getData() <= 0.0f && _azimuth->getData() >= 1.0)
    azimuth =0.0;
  else
    azimuth= ((2*_azimuth->getData())-1)*_rxScale;

  vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
    <<"[vtkVRJNavigation::updateAzimuth]" << "azimuth "<< azimuth
    <<vprDEBUG_FLUSH;

  gmtl::preMult ( _navMatrix,
                  gmtl::makeRot<gmtl::Matrix44f>
                  (gmtl::EulerAngleXYZf
                   ( 0.0f,azimuth, 0.0f )));
}

void vtkVRJNavigation::updateElevation()
{
  float elevation;
  if(_elevation->getData() <= 0.0f && _elevation->getData() >= 1.0)
    elevation =0.0;
  else
    elevation = ((2*_elevation->getData())-1)*_rxScale;


  vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
    <<"[vtkVRJNavigation::updateElevation]" << "elevation "<< elevation
    <<vprDEBUG_FLUSH;

  gmtl::preMult ( _navMatrix,
                  gmtl::makeRot<gmtl::Matrix44f>
                  (gmtl::EulerAngleXYZf
                   ( elevation,0.0f, 0.0f )));

}

void vtkVRJNavigation::updateYaw()
{
  float yaw = _yaw->getData();
  printf ("yaw %f\n",yaw);
  gmtl::preMult ( _navMatrix,
                  gmtl::makeRot<gmtl::Matrix44f>
                  (gmtl::EulerAngleXYZf
                   ( 0.0f,yaw, 0.0f )));
}

void vtkVRJNavigation::updatePitch()
{
  float pitch = _pitch->getData();
  printf("pitch %f\n",pitch);
  gmtl::preMult ( _navMatrix,
                  gmtl::makeRot<gmtl::Matrix44f>
                  (gmtl::EulerAngleXYZf
                   ( pitch,0.0f, 0.0f )));

}

void vtkVRJNavigation::updateRoll()
{
  float roll = (2*_roll->getData())-1;
}

const gmtl::Matrix44f& vtkVRJNavigation::navigate()
{
  printf ("_______________________________________\n");
  return _navMatrix;

}

bool vtkVRJNavigation::isButtonPressed(int i)
{
  if(i<=0 && i>=11)
    return false;
  else
    return(_b[i-1]->getData()==gadget::Digital::ON);
}

void vtkVRJNavigation::setBounds(double bounds[6])
{
   for(int i=0;i<6;i++)
    _bounds[i]=bounds[i];

   _center[0]=(bounds[0]+bounds[1])/2;
   _center[1]=(bounds[2]+bounds[3])/2;
   _center[2]=(bounds[4]+bounds[4])/2;
}








