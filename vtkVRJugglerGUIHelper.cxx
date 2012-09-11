//==========================================================================nix
/**
 * @file   vtkVRJugglerGUIHelper.cxx
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 03:37:40 2010
 *
 * @brief Methods for the class vtkVRJugglerGUIHelper are defined here.
 *
 * @todo Mark all public methods public at end of lines and likewise.
 */
//==========================================================================nix

//---------------------------------------------------------------------includes
#include "vtkVRJugglerGUIHelper.h"
#include "vtkInitializationHelper.h"
#include <math.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OpenGL/ProcApp.h>
#include <GL/gl.h>
#include <vtkPVXMLParser.h>
#include "vtkSMSessionProxyManager.h"
#include "vtkSMRenderViewProxy.h"
#include "vtkObjectFactory.h"
#include "vtkProcessModule.h"
#include "vtkPVConfig.h"
#include "vtkSMSession.h"
#include "vtkVRJugglerOptions.h"
#include "vtkSMSessionClient.h"
#include "vtkNetworkAccessManager.h"
#include "vtkProcessModule.h"
#include "vtkSMProxySelectionModel.h"
#include "vtkSMCollaborationManager.h"

using namespace vrj;


//vtkCxxRevisionMacro(vtkVRJugglerGUIHelper, "$Revision: 1.1$");
vtkStandardNewMacro(vtkVRJugglerGUIHelper);
vpr::Mutex mutex;

#include <iostream>
#include <sstream>
struct Connection
{
  Connection(std::string host, std::string port)
    {
    std::stringstream ss;
    ss <<"cs://"<<host<<":"<<port;
    this->Session = vtkSMSessionClient::New();
    this->Session->Connect(ss.str().c_str());
    this->ID = vtkProcessModule::GetProcessModule()->RegisterSession(this->Session);
    }

  virtual ~Connection()
  {
    this->Session->Delete();
  }

  vtkSMSessionClient* session(){
    return this->Session;
  }

  vtkSMSessionClient *Session;
  vtkIdType ID;
};

Connection *ActiveConnection =NULL;

Connection* connect(std::string host, std::string port)
{
  ActiveConnection = new Connection(host,port);
  return ActiveConnection;
}

vtkSMProxySelectionModel* selectionModel(std::string name)
{
  if(!ActiveConnection->session())
    {
    std::cout<<"Active Session not found."<<std::endl;
    exit(1);
    }
  vtkSMSessionClient *session = ActiveConnection->session();
  vtkSMSessionProxyManager* pxm = session->GetSessionProxyManager();
  vtkSMProxySelectionModel* model = pxm->GetSelectionModel(name.c_str());
  if (model == NULL)
    {
    std::cout << name << " not found" << std::endl;
    model = vtkSMProxySelectionModel::New();
    pxm->RegisterSelectionModel(name.c_str(), model);
    model->FastDelete();
    }
  return model;
}

vtkSMViewProxy* activeView()
{
  return vtkSMViewProxy::SafeDownCast(selectionModel("ActiveView")->GetCurrentProxy());
}

vtkSMSourceProxy* activeSource()
{
  return vtkSMSourceProxy::SafeDownCast(selectionModel("ActiveSources")->GetCurrentProxy());
}

#if 0
---------------------------
  vtkSMProxyManager* pxm = vtkSMProxyManager::GetProxyManager();//->GetActiveSessionProxyManager();
  pxm->SetActiveSession(sessionID);
  // pxm->AddObserver((9753,callback);

  vtkSMSessionProxyManager* spxm = vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
  spxm->UpdateFromRemote();

  if(pxm->GetActiveSession())
    {
    vtkSMSessionClient* session = vtkSMSessionClient::SafeDownCast(pxm->GetActiveSession());
    if(session->IsMultiClients()&& session->IsNotBusy())
      {
      std::cout<< "Processing remote events" <<std::endl;
      while(vtkProcessModule::GetProcessModule()->GetNetworkAccessManager()->ProcessEvents(100));
      }
    }
#endif

bool newData;
void update()
{
  vtkSMSessionClient* session = vtkSMSessionClient::SafeDownCast(vtkSMProxyManager::GetProxyManager()->GetActiveSession());
  if(session->IsMultiClients()&& session->IsNotBusy())
    {
    bool updated = false;
    while(vtkProcessModule::GetProcessModule()->GetNetworkAccessManager()->ProcessEvents(100))
      {
      updated = true;
     // std::cout<<"Update ..."<<std::endl;
      }
    if(updated)
      {
      //_renderer->reload();
      //vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager()->UpdateFromRemote();
      vtkSMRenderViewProxy* rvp = vtkSMRenderViewProxy::SafeDownCast(
            vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager()->
            GetSelectionModel("ActiveView")->GetCurrentProxy());
      mutex.acquire();
      rvp->UpdateVTKObjects();
      rvp->StillRender();
      newData = true;
      mutex.release();
      }
    }
}

//----------------------------------------------------------------------juggler
/**
 * Callback in VRJuggler. Used to initialize.
 *
 */
void vtkVRJugglerGUIHelper::init()
{
  double center[3],bounds[6];
  std::cout << "---------- App:init() ---------------" << std::endl;
  mWand.init("VJWand");
  mHead.init("VJHead");

  // Initialize paraview

  //loadRemoteParaviewState();
  //loadParaviewState();
//  _rot = new vtkVRJNavigation(ROTATION);
//  _trans = new vtkVRJNavigation(TRANSLATION);
//  mGamePad = new GamepadNavigation(this);
//  mKeyboardMouse = new KeyBoardMouseNavigation(this);
  //mGamePad->initialize      ();
  //mKeyboardMouse->initialize();



  gmtl::identity(mNavMatrix);
  gmtl::identity(_rotationMatrix);
//  gmtl::Vec3f trans(-55000,-6664740,-822);
//  gmtl::setTrans(mNavMatrix, trans);
}

//----------------------------------------------------------------------juggler
/**
 * Context specific initilizations are done here.
 *
 */
void vtkVRJugglerGUIHelper::contextInit()
{
  initGLState();
  vrj::opengl::DrawManager* drawMan = dynamic_cast<vrj::opengl::DrawManager*> ( this->getDrawManager() );
  _renderer->setDrawManager(drawMan);
  _renderer->contextInit();
  vrj::Projection::setNearFar(1,10000000);
}

//-----------------------------------------------------------------------method
/**
 * load the statefile
 *
 */
void vtkVRJugglerGUIHelper::loadParaviewState()
{
  //get hold of proxy manager so we can start instantiating templates
  if(!this->Options->GetStateFile())
    {
    vtkErrorMacro("No state to load");
    return;
    }

  vtkPVXMLParser *parser = vtkPVXMLParser::New();
  ifstream ifp;
  ifp.open(this->Options->GetStateFile(), ios::in | ios::binary);

  // get length of file.
  ifp.seekg(0, ios::end);
  int length = ifp.tellg();
  ifp.seekg(0, ios::beg);

  char* buffer = new char[length+1];
  ifp.read(buffer, length);
  buffer[length] = 0;
  ifp.close();

  // Replace ${DataDir} with the actual data dir path.
  std::string str_buffer (buffer);
  delete []buffer;
  buffer = 0;

  parser->Parse(str_buffer.c_str(), str_buffer.length());

  vtkSMSession* session = vtkSMSession::New();
  vtkProcessModule::GetProcessModule()->RegisterSession(session);
  session->Delete();
  vtkSMSessionProxyManager* pxm = vtkSMProxyManager::GetProxyManager()->GetSessionProxyManager(session);

  pxm->LoadXMLState(parser->GetRootElement());
  parser->Delete();

  pxm = vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
  pxm->LoadXMLState(this->Options->GetStateFile());
  std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("views")<<endl;
  std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("sources")<<endl;

  // Create View
  rvp=0;
  //rvp = vtkSMRenderViewProxy::SafeDownCast(pxm->NewProxy("views", "RenderView"));
//  vtkSMProxy * proxy=0;
  //vtkCollection *collection=vtkCollection::New();
  rvp= vtkSMRenderViewProxy::SafeDownCast(pxm->GetProxy("views","RenderView1"));
//    pxm->GetProxies("views", "RenderView",collection);
  if(0==rvp)
    {
    printf ("nothing in collection \n");
    _Exit(1);
    }
  pxm->RegisterProxy("view","RenderView",rvp);
  _renderer = new vtkVRJugglerRenderer(rvp);
  _renderer->OffScreenRenderingOn();
  rvp->UpdateVTKObjects();
  rvp->ResetCamera();
  //rvp->StillRender();
  //pxm->SaveState("cow.pvsm");
  //vtksys::SystemTools::Delay(5000);
}

//-----------------------------------------------------------------------method
/**
 * load the statefile
 *
 */
void vtkVRJugglerGUIHelper::loadRemoteParaviewState()
{

#if 0
  // Connect to the remote server
  //_cid = this->ProcessModule->ConnectToRemote("localhost",11111);

  //get hold of proxy manager so we can start instantiating templates
  pxm = vtkSMObject::GetProxyManager();
  pxm->LoadState(options->GetStateFile());
  std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("views")<<endl;
  std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("sources")<<endl;
  // Create View
    vtkSMRenderViewProxy* rvp=0;
  //rvp = vtkSMRenderViewProxy::SafeDownCast(pxm->NewProxy("views", "RenderView"));
  vtkSMProxy * proxy=0;
  //vtkCollection *collection=vtkCollection::New();
  rvp= vtkSMRenderViewProxy::SafeDownCast(pxm->GetProxy("views","RenderView1"));
//    pxm->GetProxies("views", "RenderView",collection);
  if(0==rvp)
    {
    printf ("nothing in collection \n");
    _Exit(1);
    }
  pxm->RegisterProxy("view","RenderView",rvp);
  _renderer = new vtkVRJugglerRenderer(rvp);
  rvp->UpdateVTKObjects();
  rvp->ResetCamera();
  //rvp->StillRender();
  //pxm->SaveState("cow.pvsm");
  //vtksys::SystemTools::Delay(5000);
#endif
}

//----------------------------------------------------------------------juggler
/**
 * Callback function called before every frame. Used to collect data
 * from input devices.
 *
 */
void vtkVRJugglerGUIHelper::preFrame()
{
  mutex.acquire();
  //_rot->update();
  //_trans->update();
//  if(_trans->isButtonPressed(10))
//    {
//    Kernel::instance()->stop();

//    }
//  if(_trans->isButtonPressed(1))
//    {
//    gmtl::identity(mNavMatrix);
//    gmtl::identity(_rotationMatrix);
//    }

//  gmtl::preMult(mNavMatrix, _trans->navigate());
//  std::cout <<mNavMatrix<<std::endl;
//  gmtl::preMult(_rotationMatrix, _rot->navigate());
//  std::cout<<_rotationMatrix<<std::endl;

  // mGamePad->update();
  // mGamePad->navigate(mNavMatrix);
//  mKeyboardMouse->update();
//  mKeyboardMouse->navigate(mNavMatrix);
}

//-----------------------------------------------------------------------juggler
/**
 * Callback in VRJuggler. Clears the viewport.  Put the call to
 * glClear() in this method so that this application will work with
 * configurations using two or more viewports per display window.
 *
 */
void vtkVRJugglerGUIHelper::bufferPreDraw()
{
  //glClearColor(56.0f/255.0, 71.0f/255.0, 92.0f/255.0, 0.0f);
  glClearColor(45.0f/255.0, 71.0f/255.0, 108.0f/255.0, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

//----------------------------------------------------------------------juggler
/**
 * Draw callback in VRJuggler
 *
 */
void vtkVRJugglerGUIHelper::draw()
{
  double bounds[6];
 _renderer->getBounds(bounds);
  double x = (bounds[0]+bounds[1])/2;
  double y = (bounds[2]+bounds[3])/2;
  double z = (bounds[4]+bounds[5])/2;
  double scale=x;
  if(y>scale) scale =y;
  if(z>scale) scale =z;
  if(scale<0) scale*=-1;

  double x1 = bounds[0];
  double x2 = bounds[1];
  double y1 = bounds[2];
  double y2 = bounds[3];
  double z1 = bounds[4];
  double z2 = bounds[5];

  double radius=sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    {
    glMultMatrixf(mNavMatrix.mData);
    glTranslatef(.0,.0,-radius);
    glPushMatrix();
      {
      glEnable(GL_LIGHTING);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT2);
      glClear(GL_DEPTH_BUFFER_BIT);

      glMultMatrixf(_rotationMatrix.mData);
      glTranslatef(-x,-y,-z);

     // glScalef(1/scale,1/scale,1/scale);

      // forward draw event to renderer
      glPushMatrix();
        {

       _renderer->draw();
        }glPopMatrix();
      } glPopMatrix();
    } glPopMatrix();
}
//----------------------------------------------------------------------juggler
/**
 * A function which gets the display manager
 *
 * @return The current display manager reference
 */
vrj::DisplayManager* vtkVRJugglerGUIHelper::getDisplayManager()
{
  return getDrawManager()->getDisplayManager();
}




//----------------------------------------------------------------------juggler
/**
 * To initiate the opengl state in vrjuggler. Convenience routine
 *
 */
void vtkVRJugglerGUIHelper::initGLState()
{
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7f, 0.7f,  0.7f, 1.0f };
   GLfloat mat_diffuse[] = { 1.0f,  0.5f,  0.8f, 1.0f };
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
//   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

//----------------------------------------------------------------------con/dst
/*
 * Constructor and Destructor
 *
 */
vtkVRJugglerGUIHelper::vtkVRJugglerGUIHelper()
{
  this->Options = vtkVRJugglerOptions::New();
}

vtkVRJugglerGUIHelper::~vtkVRJugglerGUIHelper()
{
  this->Options->Delete();
  this->Options=0;
  vtkInitializationHelper::Finalize();
}

void vtkVRJugglerGUIHelper::intraFrame()
{
}
bool callback(vtkObject*,long int event)
{
  std::cout<<"Called Back>>>>>>>>>>>>>>>>"<<std::endl;
}

void vtkVRJugglerGUIHelper::Initialize(int argc, char *argv[])
{
  vtkInitializationHelper::Initialize(argc,argv,
                                      vtkProcessModule::PROCESS_CLIENT,
                                      this->Options);
  std::cout<< "host:port = "
           << this->Options->GetDataServerHostName()<<":"<<this->Options->GetDataServerPort()
           <<std::endl;
#if 1
  vtkIdType sessionID = vtkSMSession::ConnectToRemote(this->Options->GetDataServerHostName(),
                                                    this->Options->GetDataServerPort());
  //  vtkIdType session1ID = vtkSMSession::ConnectToSelf();
    vtkSMProxyManager* pxm = vtkSMProxyManager::GetProxyManager();//->GetActiveSessionProxyManager();
    pxm->SetActiveSession(sessionID);
    if(pxm->GetActiveSession())
      {
      vtkSMSessionClient* session = vtkSMSessionClient::SafeDownCast(pxm->GetActiveSession());

#else
  vtkSMProxyManager::GetProxyManager()
      ->SetActiveSession(vtkSMSessionClient::ConnectToRemote(this->Options->GetDataServerHostName(),
                                                             this->Options->GetDataServerPort()));
  vtkIdType sessionID = vtkProcessModule::GetProcessModule()->RegisterSession(session);
  vtkSMSessionProxyManager* spxm;
  if(session)
    {
    spxm = session->GetSessionProxyManager();//vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
    std::cout<<"connected"<<spxm<<std::endl;

    //spxm->UpdateFromRemote();
#endif

    if(session->IsMultiClients()&& session->IsNotBusy())
      {
      std::cout<< "Processing remote events" <<std::endl;
      while(vtkProcessModule::GetProcessModule()->GetNetworkAccessManager()->ProcessEvents(100));
      }
    }
  vtkSMSessionProxyManager* spxm = vtkSMProxyManager::GetProxyManager()->GetActiveSessionProxyManager();
  std::cout<<"connected"<<spxm<<std::endl;
  spxm->UpdateFromRemote();
  // setup the active-view and active-sources selection models.
  vtkSMProxySelectionModel* selmodel = spxm->GetSelectionModel("ActiveSources");
  if (selmodel == NULL)
    {
    std::cout << "Active Sources not found" << std::endl;

    selmodel = vtkSMProxySelectionModel::New();
    spxm->RegisterSelectionModel("ActiveSources", selmodel);
    selmodel->FastDelete();
    }
  this->ActiveSources = vtkSMSourceProxy::SafeDownCast(selmodel->GetCurrentProxy());
  selmodel = spxm->GetSelectionModel("ActiveView");
  if (selmodel == NULL)
    {
    std::cout << "Active View not found" << std::endl;

    selmodel = vtkSMProxySelectionModel::New();
    spxm->RegisterSelectionModel("ActiveView", selmodel);
    selmodel->FastDelete();
    }
  this->ActiveView = vtkSMViewProxy::SafeDownCast(selmodel->GetCurrentProxy());

  //  vtkSMProxy* sphere1 = pxm->NewProxy("sources", "SphereSource");
//  sphere1->UpdateVTKObjects();
//  vtkSMProxy* view1 = pxm->NewProxy("views", "RenderView");
//  view1->UpdateVTKObjects();
//  vtkSMRenderViewProxy::SafeDownCast(view1)->StillRender();
//  int foo; cin >> foo;
//  exit();
//  sphere1->Delete();
//  view1->Delete();
  vtkSMRenderViewProxy *rvp = 0;
//  vtkSMViewProxy *vp;
  rvp=vtkSMRenderViewProxy::SafeDownCast(selmodel->GetCurrentProxy());
  //vp=vtkSMRenderViewProxy::SafeDownCast(rvp);
  //vp->EnableOff();
  rvp->StillRender();
  //vtkSMRenderViewProxy::SafeDownCast(pxm->GetProxy("views","RenderView"));
  if(rvp)
    {
    _renderer = new vtkVRJugglerRenderer(rvp);//vtkSMRenderViewProxy::SafeDownCast(view1));
    _renderer->OffScreenRenderingOn();
    rvp->UpdateVTKObjects();
    rvp->ResetCamera();
    //rvp->StillRender();
    }
  else
    {
    cout<<" Render view proxy not found" <<std::endl;
    int temp;
    cin >> temp;
    return;
    }

  // Load the state into kernel and process it.
  //this->VRKernel->init(argc,argv);
  this->VRKernel = Kernel::instance();
  this->VRKernel->init(argc,argv);
  if(this->Options->GetJugglerConfigurationFile())
    {
    std::cout<< this->Options->GetJugglerConfigurationFile() << std::endl;
    }
  else
    {
    std::cout << "***********************************************************" <<std::endl;
    }
  this->VRKernel->loadConfigFile(this->Options->GetJugglerConfigurationFile());
  this->VRKernel->start();
  this->VRKernel->setApplication(this);
}

//-----------------------------------------------------------------------public
/**
 * Used to print the values in the class in the desired format to a given stream
 *
 * @param os output stream ex std::cout
 * @param indent indentation type
 */
void vtkVRJugglerGUIHelper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

class Callable
{
public:
   Callable()
      : mDoWork(true)
   {
   }

   void stop()
   {
      mDoWork = false;
   }

   void operator()()
   {
      while ( mDoWork )
      {
  Kernel::instance()->waitForKernelStop();       // Do work ...
      }
   }

private:
   bool mDoWork;
};
void loop()
{

}

//-----------------------------------------------------------------------public
int vtkVRJugglerGUIHelper::Exec()
{
  Callable Functor;
  new vpr::Thread(Functor);
  while(1)update();
  return 0;
}

void vtkVRJugglerGUIHelper::postFrame()
{
  mutex.release();
//  if(_trans->isButtonPressed(2))
//    {
//    pxm->UnRegisterProxies();
    //pxm->LoadState(options->GetStateFile());

//    std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("views")<<endl;
//    std::cout << "Total number of proxies = " << pxm->GetNumberOfProxies("sources")<<endl;
//    rvp=0;
//    rvp= vtkSMRenderViewProxy::SafeDownCast(pxm->GetProxy("views","RenderView1"));
//    if(0==rvp)
//      {
//      printf ("nothing in collection \n");
//      _Exit(1);
//      }
//    pxm->RegisterProxy("view","RenderView",rvp);
//    _renderer->resetRenderViewProxy(rvp);
//    rvp->UpdateVTKObjects();
//    rvp->UpdateVTKObjects();
//    rvp->ResetCamera();
//    _renderer->reload();
////    }
 // update();
//  this->ActiveView->StillRender();
//
//  rvp->StillRender();
  //_renderer->resetRenderViewProxy(rvp);
}

void vtkVRJugglerGUIHelper::contextPreDraw()
{
   _renderer->contextPostFrame(newData);
   newData = false;
}

