//==========================================================================nix
/**
 * @file   main.cxx
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 03:49:55 2010
 *
 * @brief This is where the program starts
 */
//==========================================================================nix


//---------------------------------------------------------------------includes
#include "vtkVRJugglerGUIHelper.h"

////-------------------------------------------------------------------------func
///**
// * Initialize client server wrapper modules.
// *
// * @param pm processmodule instance
// */
//void ParaViewInitializeInterpreter(vtkProcessModule* pm)
//{
//  vtkInitializationHelper::InitializeInterpretor(pm);
//}

//-------------------------------------------------------------------------main
/**
 * MAIN This is where is all starts
 *
 * @param argc Total number of input parameters
 * @param argv Array of input strings
 *
 * @return 0 on success and negative numbers on error
 */
int main(int argc, char* argv[])
{
  vtkVRJugglerGUIHelper *app = vtkVRJugglerGUIHelper::New();
  app->Initialize(argc,argv);
  app->Exec();
  app->Delete();
#if OLD
  vtkPVMain::SetUseMPI(0);             // don't use MPI at all.
  vtkPVMain::Initialize(&argc, &argv); // MPI must be initialized
                                       // before any vtk object
  vtkPVMain* pvmain = vtkPVMain::New();
  options->SetProcessType(vtkPVOptions::PVCLIENT);
  vtkVRJugglerGUIHelper* helper = vtkVRJugglerGUIHelper::New();
  int ret = pvmain->Initialize(options,
                               helper,
                               ParaViewInitializeInterpreter,
                               argc, argv);
  if (!ret)
    {
    ret = pvmain->Run(options);
    }
  helper->Delete();
  pvmain->Delete();
  options->Delete();
  vtkPVMain::Finalize();
  return ret;
#endif
}

