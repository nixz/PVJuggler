//==========================================================================nix
/**
 * @file   vtkVRJugglerOptions.cxx
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 01:05:27 2010
 *
 * @brief  This class is used to load command line options into the
 * paraview/juggler client. Only one file can be loaded with the
 * following parameters --VRJConfig="test.jconf" or -vr="test.jconf"
 */
//==========================================================================nix

//---------------------------------------------------------------------includes
#include "vtkVRJugglerOptions.h"
#include "vtkObjectFactory.h"
#include <vtksys/SystemTools.hxx>
#include <vtksys/ios/sstream>


vtkStandardNewMacro(vtkVRJugglerOptions);

//----------------------------------------------------------------------con/des
/*
 * Constructors and Destructor
 */
vtkVRJugglerOptions::vtkVRJugglerOptions():vtkPVOptions()
{
  this->ServerMode = 0;
  this->JugglerConfigurationFile=0;
  this->StateFile=0;
}

vtkVRJugglerOptions::~vtkVRJugglerOptions()
{
}

//-----------------------------------------------------------------------method
/**
 * This is the initialization routine. We can add custom command line
 * keys here. --VRJConfig or -vr is registered. This will show up when
 * --help is passed at the command line
 *
 */void vtkVRJugglerOptions::Initialize()
{
  this->Superclass::Initialize();
  this->AddArgument("--VRJConfig", "-vr", &JugglerConfigurationFile,
                    "VRJuggler configuration file");
  this->AddArgument("--StateFile", "-sf", &StateFile,
                    "Saved State file");
}

//-----------------------------------------------------------------------method
/**
 * This can be used to perform any post processing task on the command
 * line arguments.
 *
 * @param argc command line argc
 * @param argv command line argv
 *
 * @return whatever is returned by the superclass PostProcess
 */
int vtkVRJugglerOptions::PostProcess(int argc, const char* const* argv)
{
  if(this->JugglerConfigurationFile &&
     vtksys::SystemTools::GetFilenameLastExtension(this->JugglerConfigurationFile) != ".jconf")
    {
    vtksys_ios::ostringstream str;
    str << "Wrong Juggler Configuration file name: "<< this->JugglerConfigurationFile <<ends;
    this->SetErrorMessage(str.str().c_str());
    return 0;
    }
  if(this->StateFile &&
     vtksys::SystemTools::GetFilenameLastExtension(this->StateFile) != ".pvsm")
    {
    vtksys_ios::ostringstream str;
    str << "Wrong ParaView State file name: "<< this->StateFile <<ends;
    this->SetErrorMessage(str.str().c_str());
    return 0;
    }
  return this->Superclass::PostProcess(argc, argv);
}

//-----------------------------------------------------------------------method
/**
 * So far found no use to this.
 *
 * @param argument this is called when there is a parse error oncommandline args
 *
 * @return whatever is returned by the superclass WrongArgument
 */
int vtkVRJugglerOptions::WrongArgument(const char* argument)
{
  return this->Superclass::WrongArgument(argument);
}

//-----------------------------------------------------------------------method
/**
 * Used to preety print the contents of the class. Here it is the
 * configuration file name which is be loaded by juggler.
 *
 * @param os the output stream ex "std::cout"
 * @param indent the type of indentation to be used
 */
void vtkVRJugglerOptions::PrintSelf(ostream& os, vtkIndent indent)
{
  os << "Juggler Configuration File =" << JugglerConfigurationFile << std::endl;
  os << "State File =" << StateFile << std::endl;
  this->Superclass::PrintSelf(os, indent);
}
