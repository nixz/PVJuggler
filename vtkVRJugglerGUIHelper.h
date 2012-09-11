//==========================================================================nix
/**
 * @file   vtkVRJugglerGUIHelper.h
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 01:33:24 2010
 *
 * @brief defines the class vtkVRJugglerGUIHelper. This is the
 * starting point of the GUI and in out case VRJuggler.
 *
 * @todo Seperate this big classes into two. One which handles GUI
 * stuff and other that handles VRJuggler callbacks.
 */
//==========================================================================nix

#ifndef VTKVRJUGGLERGUIHELPER_H_
#define VTKVRJUGGLERGUIHELPER_H_

//---------------------------------------------------------------------includes
//#include "vtkProcessModuleGUIHelper.h"
#include "vtkVRJugglerOptions.h"
#include "vtkObjectFactory.h"
#include "vtkProcessModule.h"
#include "vtkSMSession.h"
//#include "vtkSMDataRepresentationProxy.h"
#include "vtkSMDoubleVectorProperty.h"
#include "vtkSMIntVectorProperty.h"
#include "vtkSMProperty.h"
#include "vtkSMPropertyAdaptor.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyProperty.h"
#include "vtkSMRepresentationProxy.h"
#include "vtkSMSourceProxy.h"
#include "vtkRenderWindow.h"
#include "vtkSMStringVectorProperty.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActorCollection.h"
#include "vtkRendererCollection.h"
#include "vtkWindows.h"
#include <vtksys/SystemTools.hxx>
//#include <vtkstd/string>
#include "vtkSMProxyManager.h"
#include "vtkSMRenderViewProxy.h"
#include "vtkVRJugglerRenderer.h"
//#include <Navigation/GamepadNavigation.h>
//#include <Navigation/KeyBoardMouseNavigation.h>
//#include <Navigation/vtkVRJNavigation.h>
#include <vrj/Draw/OpenGL/App.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <GL/gl.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vrj/Display/DisplayManager.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>


using namespace gmtl;
using namespace vrj;
//using namespace Navigation;

class vtkSMApplication;
class vtkVRJugglerRenderer;

//------------------------------------------------------------------------class
/*
 * @class vtkVRJugglerGUIHelper
 *
 * @brief The role of this class is to establish gui facility.
 */
class vtkVRJugglerGUIHelper : public vtkObject, public vrj::opengl::App
{
public:
  static vtkVRJugglerGUIHelper* New();
  vtkTypeMacro(vtkVRJugglerGUIHelper,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);
  void Initialize(int argc, char* argv[]);

  virtual int Exec();
//  virtual int OpenConnectionDialog(int*) { return 1; }
//  virtual void SendPrepareProgress();
//  virtual void SendCleanupPendingProgress();
//  virtual void SetLocalProgress(const char*, int);
//  virtual void ExitApplication();
public:
  virtual void init();
  virtual void apiInit(){;}
  virtual void bufferPreDraw();
  virtual void preFrame();
  virtual void intraFrame();
  virtual void postFrame();
  virtual void contextInit();
  virtual void contextPreDraw();
  virtual void draw();
  vrj::DisplayManager* getDisplayManager();
public:
  gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
  gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
private:
  void loadParaviewState();
  void loadRemoteParaviewState();
  void initGLState();
protected:
  vtkVRJugglerGUIHelper();
  virtual ~vtkVRJugglerGUIHelper();
  vtkVRJugglerOptions* Options;
  Kernel *VRKernel;

  vtkSMApplication* SMApplication;
  int ShowProgress;
  // vtkSetStringMacro(Filter);
  char* Filter;
  int CurrentProgress;
  vtkSMSessionProxyManager* pxm;
  vtkSMSession *session;
  vtkSMRenderViewProxy* rvp;
  vtkSMSourceProxy* ActiveSources;
  vtkSMViewProxy* ActiveView;
//  KeyBoardMouseNavigation *mKeyboardMouse;
//  GamepadNavigation *mGamePad;
//  vtkVRJNavigation *_rot;
//  vtkVRJNavigation *_trans;
  Matrix44f mNavMatrix;
  Matrix44f _rotationMatrix;

  vtkVRJugglerRenderer *       _renderer;
  vtkIdType _cid;
  void CloseCurrentProgress();
private:
  vtkVRJugglerGUIHelper(const vtkVRJugglerGUIHelper&); // Not implemented
  void operator=(const vtkVRJugglerGUIHelper&); // Not implemented
};

#endif /*!VTKVRJUGGLERGUIHELPER_H_*/
