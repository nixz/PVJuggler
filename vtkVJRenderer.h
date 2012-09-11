/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVJRenderer.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkVJRenderer -
// .SECTION Description
// vtkVJRenderer

#ifndef __vtkVJRenderer_h
#define __vtkVJRenderer_h
// --------------------------------------------------------------------includes
#include "vtkOpenGLRenderer.h"

// -----------------------------------------------------------------pre-defines
class vtkVJRendererInternal;
class vtkOpenGLRenderer;
// -----------------------------------------------------------------------class
class vtkVJRenderer : public vtkOpenGLRenderer
{
public:
  // ............................................................public-methods
  static vtkVJRenderer* New();
  vtkTypeMacro(vtkVJRenderer, vtkOpenGLRenderer);
  void PrintSelf(ostream& os, vtkIndent indent);

  int UpdateGeometry();
protected:
  // ...........................................................protected-ivars

protected:
//BTX
  // .......................................................................BTX
  vtkVJRenderer();
  ~vtkVJRenderer();

private:
  vtkVJRendererInternal *Internal;
  vtkVJRenderer(const vtkVJRenderer&); // Not implemented.
  void operator=(const vtkVJRenderer&); // Not implemented.
//ETX
  // .......................................................................ETX


};

#endif // __vtkVJRenderer_h
