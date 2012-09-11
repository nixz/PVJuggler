//==========================================================================nix
/**
 * @file   vtkVRJugglerRenderer.cxx
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 01:28:39 2010
 *
 * @brief  Contains method which help in rendering to a juggler window.
 */
//==========================================================================nix


//---------------------------------------------------------------------includes
#include "vtkVRJugglerRenderer.h"
#include <vtkSMProxyManager.h>

#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Draw/OpenGL/UserData.h>
#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Display/Viewport.h>
#include <vtkVJRenderer.h>

using namespace std;
using namespace vrj;

//----------------------------------------------------------------------con/des
/*
 * Constructors and Destructor
 */
vtkVRJugglerRenderer::vtkVRJugglerRenderer()
{
  _drawManager = NULL;
  _reload=false;
}

vtkVRJugglerRenderer::~vtkVRJugglerRenderer()
{
}

//-----------------------------------------------------------------------method
/**
 * Public method which should be use to send the draw manager form the
 * application
 *
 * @param drawManager drawmanager instance sent form vrjuggler app
 */
void vtkVRJugglerRenderer::setDrawManager (vrj::opengl::DrawManager * drawManager)
{
  _drawManager = drawManager;
  // _nav = new vtkVRJNavigation();
}

//-----------------------------------------------------------------------public
/**
 * This method is used to get the center of the bounding volume. It
 * takes the global maximum and minimum values of x,y and z
 * respectively and returns the global center.
 *
 * @param center Returns the values of the center of the bounding volume.
 */
void vtkVRJugglerRenderer::getCenter (double center[3])
{
  double bounds[6];
  getBounds(bounds);
  center[0] = (bounds[0]+bounds[1])/2;
  center[1] = (bounds[2]+bounds[3])/2;
  center[2] = (bounds[4]+bounds[5])/2;
}

//-----------------------------------------------------------------------public
/**
 * Scans through all the actor bounds and gets the global maximum and minimum bounds
 *
 * @param bounds An arrary is returned containing xmin,xmax,ymin,ymax,zmin,zmax
 */
void vtkVRJugglerRenderer::getBounds(double bounds[6])
{
  double xmin=0.0, xmax=0.0,ymin=0.0,ymax=0.0,zmin=0.0,zmax=0.0;

vtkActor * pCurActor = NULL;
(*_actors)->InitTraversal();
do
  {
    pCurActor = (*_actors)->GetNextActor();
    if (pCurActor != NULL)
      if(pCurActor->GetMapper() !=NULL)
        {
        pCurActor->GetBounds(bounds);
        xmin = (bounds[0]<xmin)?bounds[0]:xmin;
        xmax = (bounds[1]>xmax)?bounds[1]:xmax;
        ymin = (bounds[2]<ymin)?bounds[2]:ymin;
        ymax = (bounds[3]>ymax)?bounds[3]:ymax;
        zmin = (bounds[4]<zmin)?bounds[4]:zmin;
        zmax = (bounds[5]>zmax)?bounds[5]:zmax;
        }
    } while (pCurActor != NULL);

  bounds[0]=xmin;
  bounds[1]=xmax;
  bounds[2]=ymin;
  bounds[3]=ymax;
  bounds[4]=zmin;
  bounds[5]=zmax;
}

//-----------------------------------------------------------------------method
/**
 * Initializes the vtk render window size per vrjuggler context. Also
 * other opengl and vtk context specific initilizations are done.
 */
void vtkVRJugglerRenderer::contextInit()
{
  vrj::opengl::UserData * pUserData = _drawManager->currentUserData();
  vrj::DisplayPtr pDisp = pUserData->getGlWindow()->getDisplay();
  int dispXOrigin, dispYOrigin, dispWidth, dispHeight;
  pDisp->getOriginAndSize (dispXOrigin, dispYOrigin, dispWidth, dispHeight);
  cout << " Disp. size: Org: " << dispXOrigin << ", " << dispYOrigin
       << ", W/H: " << dispWidth << ", " << dispHeight << endl;
  vtkVRJugglerRenderer::SetSize (dispWidth, dispHeight);
  contextInitOpenGL();
  contextInitVtk ();
  // _nav->setBounds(bounds);
}

//-----------------------------------------------------------------------method
/**
 * Private method to do opengl context initialization per rendering
 * context in vrjuggler.
 */
void vtkVRJugglerRenderer::contextInitOpenGL()
{
  GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat light1_position[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat light2_position[] = { -1.0, 0.0, -1.0, 0.0 };
  GLfloat light3_position[] = { -1.0, -5.0, -1.0, 0.0 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light1_position);

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light2_position);

  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT2, GL_POSITION, light3_position);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glDepthRange( 0.0, 1.0 );
  glShadeModel( GL_SMOOTH );
  glFrontFace( GL_CCW );
  glDepthFunc( GL_LEQUAL );
  glAlphaFunc( GL_GREATER, 0.35);

  glEnable( GL_DEPTH_TEST );
  glEnable( GL_AUTO_NORMAL );
  glEnable( GL_NORMALIZE );
}

//-----------------------------------------------------------------------method
/**
 * Private method which assigns contest specific vtk data i.e the
 * renders and actors collection.
 */
void vtkVRJugglerRenderer::contextInitVtk ()
{
  vtkVJRenderer * pRen = vtkVJRenderer::New();
  this->AddRenderer (pRen);
  pRen->GetVTKWindow()->SetSize (this->GetSize());
  vtkViewport * pVport = (vtkViewport*)pRen;
  pVport->GetVTKWindow()->SetSize (this->GetSize());
  (*_renderer) = vtkVJRenderer::SafeDownCast(pRen);
  (*_actors)=_rvp->GetRenderer()->GetActors();
  //(*_actors)->setData(_rvp->GetRenderer()->GetActors()) ;
}

void vtkVRJugglerRenderer::reload()
{
  std::cout<< "Reloading"<<std::endl;
//    if ((*_actors) != NULL)
//     {
//     (*_actors)->RemoveAllItems();
//     //(*_actors)->Delete();
//     }
//   (*_actors) = NULL;

//_rvp->UpdateVTKObjects();
std::cout<< "UpdateVTKObjects()" << std::cout;
_rvp->StillRender();
std::cout<< "StillRender()" <<std::cout;

#if 0
  vtkSMRenderViewProxy* rvp=0;
    rvp= vtkSMRenderViewProxy::SafeDownCast(vtkSMProxyManager::GetProxyManager()->GetProxy("views","RenderView1"));
    if(0==rvp)
      {
      printf ("nothing in collection \n");
      _Exit(1);
      }
#endif
//_reload = true;

}

//-----------------------------------------------------------------------method
/**
 * Method that can be called when application context is getting
 * closed. This removes all the collected actors.
 */
void vtkVRJugglerRenderer::contextClose()
{
//  if ((*_actors) != NULL)

//    {
//    (*_actors)->RemoveAllItems();
//    (*_actors)->Delete();
//    }
//  (*_actors) = NULL;


//  if ((*_renderer))
//    {
//    this->RemoveRenderer ((*_renderer));
//    (*_renderer)->Delete();
//    }
//  (*_renderer) = NULL;
}

//-----------------------------------------------------------------------method
/**
 * Draws the the actors extracted from the actors. It first draws
 * opaque geometry then draws then the translucent geometry.
 */
void vtkVRJugglerRenderer::draw ()
{
  //(*_renderer)->UpdateGeometry();
  drawOpaqueGeometry();
  drawTranslucentPolygonalGeometry();
  //this->render();
}

void vtkVRJugglerRenderer::render()
{
  this->vtkXOpenGLRenderWindow::Render();
}

//-----------------------------------------------------------------------method
/**
 * Private method used to draw opaque geometry from the actor
 * collection.
 */
void vtkVRJugglerRenderer::drawOpaqueGeometry()
{
  vtkActor * pCurActor = NULL;
  (*_actors)->InitTraversal();
  int count =0;
  do
    {
    pCurActor = (*_actors)->GetNextActor();

    if (pCurActor != NULL && pCurActor->GetVisibility())
      if(pCurActor->GetMapper() !=NULL)
        {
//        count+=1;
//        std::cout<<"rendering opaque geometry " << count <<std::endl;
        pCurActor->RenderOpaqueGeometry ((vtkViewport*)(*_renderer));
        }

    } while (pCurActor != NULL);
}

//-----------------------------------------------------------------------method
/**
 * Private method to draw translucent poly geometry formthe list of
 * actors collected
 */
void vtkVRJugglerRenderer::drawTranslucentPolygonalGeometry()
{
  vtkActor * pCurActor = NULL;
  (*_actors)->InitTraversal();

  do
    {
    pCurActor = (*_actors)->GetNextActor();

    if (pCurActor != NULL && pCurActor->GetVisibility())
      {
      pCurActor->RenderTranslucentPolygonalGeometry (vtkViewport::SafeDownCast(*_renderer));
      }
    }while (pCurActor != NULL);
}

void vtkVRJugglerRenderer::contextPostFrame(bool updated)
{
//  if(_reload){
//    std::cout<< "contextPostFrame()"<< std::endl;
#if 0
  if ((*_renderer))
    {
    this->RemoveRenderer ((*_renderer));
    (*_renderer)->Delete();
    }
  (*_renderer) = NULL;

  vtkVJRenderer * pRen = vtkVJRenderer::New();
  this->AddRenderer (pRen);
  pRen->GetVTKWindow()->SetSize (this->GetSize());
  vtkViewport * pVport = (vtkViewport*)pRen;
  pVport->GetVTKWindow()->SetSize (this->GetSize());
  (*_renderer) = vtkVJRenderer::SafeDownCast(pRen);
#endif

    (*_actors)=_rvp->GetRenderer()->GetActors();
 //   (*_props) =_rvp->GetRenderer()->GetViewProps();
//    _reload=false;
//  }
}





