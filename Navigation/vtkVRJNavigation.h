// =========================================================================nix
/**
 * @file   vtkVRJNavigation.h
 * @author Nikhil <nikhil.j.shetty@gmail.com>
 * @date   Mon Mar  1 12:48:13 2010
 *
 * @brief  VRJuggler navigation class
 */
// =========================================================================nix

#ifndef   	VTKVRJNAVIGATION_H_
# define   	VTKVRJNAVIGATION_H_

#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/PositionProxy.h>
#include <gmtl/Matrix.h>
#include <string.h>

#define TRANSLATION 0
#define ROTATION 1

// -------------------------------------------------------------------------nix
/**
 * @class vtkVRJNavigation
 *
 * @brief This class maintains the navigation matrix. Two methods
 * update() and navigate() mainly used to get update the matrix
 * and get the new navigation matrix
 */
class vtkVRJNavigation
{
public:
  vtkVRJNavigation(bool);
  virtual ~vtkVRJNavigation();
  void setBounds(double bounds[6]);
  double getCenterX(){return(_center[0]);}
  double getCenterY(){return(_center[1]);}
  double getCenterZ(){return(_center[2]);}

  void setForwardBackward(const std::string name);
  void setLeftRight(const std::string name);
  void setTopBottom(const std::string name);

  void setYaw(const std::string name);
  void setPitch(const std::string name);
  void setAzimuth(const std::string name);
  void setElevation(const std::string name);

  void setRoll(const std::string name);
  void setButtons(const std::string button01,
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
                  const std::string button12);

  // void setAzimuth(const std::string name);
  // void setElevation(const std::string name);

  void update();
  const gmtl::Matrix44f& navigate();
  bool isButtonPressed(int);
public:
  // friend ostream& operator<<(ostream& output, const vtkVRJNavigation& obj);
  // friend ostream& operator>>(ostream&  input, const vtkVRJNavigation& obj);
protected:
  void updateButtons();         // check buttons first
  void updateForwardBack();     // translate in z
  void updateLeftRight();       // translate in x
  void updateTopBottom();       // translate in y
  void updateYaw();             // rotate about y (local)
  void updatePitch();           // rotate about x (local)
  void updateRoll();            // rotate about z (local)
  void updateAzimuth();             // rotate about y (local)
  void updateElevation();           // rotate about x (local)


// void updateAzimuth();
  // void updateElevation();

protected:
  gmtl::Matrix44f _navMatrix;

  //gadget::PositionProxy _head;
  gadget::AnalogInterface _forwardBack;
  gadget::AnalogInterface _leftRight;
  gadget::AnalogInterface _topBottom;
  gadget::AnalogInterface _yaw;
  gadget::AnalogInterface _pitch;
  gadget::AnalogInterface _azimuth;
  gadget::AnalogInterface _elevation;
  gadget::AnalogInterface _roll;
  gadget::DigitalInterface _b[12];
  double _txScale,_rxScale;
  double _center[3];
  double _bounds[6];
  bool _t_or_r;
};


#endif 	    /* !VTKVRJNAVIGATION_H_ */
