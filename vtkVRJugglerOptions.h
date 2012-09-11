//==========================================================================nix
/**
 * @file   vtkVRJugglerOptions.h
 * @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
 * @date   Tue Mar 30 00:04:03 2010
 *
 * @brief This file is used to take optional paramaters
 */
//==========================================================================nix

#ifndef VTKVRJUGGLEROPTIONS_H_
#define VTKVRJUGGLEROPTIONS_H_

//---------------------------------------------------------------------includes
#include "vtkPVOptions.h"

//------------------------------------------------------------------------class
/*
 * @class vtkVRJugglerOptions
 *
 * @brief This class is used to get the vrjuggler config file from the
 * command line arguments. The parameters that one should use are
 * --VRJConfig="test.jconf" or -vr="test.jconf".
 */
class vtkVRJugglerOptions : public vtkPVOptions
{
public:
  static vtkVRJugglerOptions* New();
  vtkTypeMacro(vtkVRJugglerOptions,vtkPVOptions);
  void PrintSelf(ostream& os, vtkIndent indent);
  vtkGetStringMacro(JugglerConfigurationFile);
  vtkGetStringMacro(StateFile);

protected:
  vtkVRJugglerOptions();
  virtual ~vtkVRJugglerOptions();
  virtual void Initialize();
  virtual int PostProcess(int argc, const char* const* argv);
  virtual int WrongArgument(const char* argument);

  vtkSetStringMacro(JugglerConfigurationFile);
  vtkSetStringMacro(StateFile);

  char* JugglerConfigurationFile;
  char* StateFile;
private:
  vtkVRJugglerOptions(const vtkVRJugglerOptions&); // Not implemented
  void operator=(const vtkVRJugglerOptions&); // Not implemented
};

#endif /*!VTKVRJUGGLEROPTIONS_H_*/


