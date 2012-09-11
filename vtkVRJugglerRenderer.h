//=============================================================================
/**
 * @file   vtkVRJugglerRenderer.h
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Wed Mar  3 05:53:09 2010
 *
 * @brief The description of vtkVRJugglerRender class is contained in this file.
 */
//=============================================================================

#ifndef   	VTKVRJUGGLERRENDERER_H_
# define   	VTKVRJUGGLERRENDERER_H_

//---------------------------------------------------------------------includes
#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/Thread.h>
#include <vrj/Draw/OpenGL/ContextData.h>
#include <vtkSMRenderViewProxy.h>
#include <vtkXOpenGLRenderWindow.h>
#include <vtkActorCollection.h>
#include <vtkRenderer.h>
#include <iostream>
#include <string>
#include "Navigation/vtkVRJNavigation.h"

class vtkActorCollection;
class vtkVJRenderer;
typedef struct
{
  vtkActorCollection* data;
  void setData(vtkActorCollection* temp) {data=temp;}
}vrjActorCollection;

//------------------------------------------------------------------------class
/*
 * @class vtkVRJugglerRenderer
 *
 * @brief This class overloads the default windowing of VTK and
 * renders in VRJuggler context instead. It is supplied with a
 * RenderViewProxy. It extracts actors form the proxy and renders it
 * in the appropriate draw routine. It also has corresponding routines
 * that can be called called in a VRJuggler application.
 */
class vtkVRJugglerRenderer : public vtkXOpenGLRenderWindow
{
public:
  vtkVRJugglerRenderer();
  vtkVRJugglerRenderer(vtkSMRenderViewProxy *a) {_rvp =a;}
  virtual ~vtkVRJugglerRenderer (void);
  //void resetRenderViewProxy(vtkSMRenderViewProxy *a) {_rvp=a;}
  void reload();
  void setDrawManager(vrj::opengl::DrawManager * drawManager);
  void getCenter(double center[3]);
  void getBounds(double bounds[6]);
  void contextInit();
  void contextPostFrame(bool update);
  void contextInitVtk ();
  void contextClose ();
  void draw ();

protected:
  void MakeCurrent() { return; } // overide window
  void drawOpaqueGeometry();
  void drawTranslucentPolygonalGeometry();
  void contextInitOpenGL ();
  void render();

protected:
  vrj::opengl::DrawManager* _drawManager;
  vrj::opengl::ContextData<vtkActorCollection*> _actors;
  vrj::opengl::ContextData<vtkPropCollection*> _props;
  vrj::opengl::ContextData<vtkVJRenderer*> _renderer;
  vtkSMRenderViewProxy* _rvp;
  bool _reload;
  // vtkVRJNavigation *_nav;
};

#endif 	    /* !VTKVRJUGGLERRENDERER_H_ */
