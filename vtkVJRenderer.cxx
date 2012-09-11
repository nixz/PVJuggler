/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVJRenderer.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkVJRenderer.h"

// --------------------------------------------------------------------includes
#include "vtkObjectFactory.h"

// -----------------------------------------------------------------------macro
// Defines standard new macro
vtkStandardNewMacro(vtkVJRenderer);

// -----------------------------------------------------------------------macro
// Needed when we don't use vtkStandardNewMacro
// vtkInstantiatorNewMacro(vtkVJRenderer);

// -----------------------------------------------------------------------cnstr
vtkVJRenderer::vtkVJRenderer()
{
}

// -----------------------------------------------------------------------destr
vtkVJRenderer::~vtkVJRenderer()
{
}

// -----------------------------------------------------------------------print
void vtkVJRenderer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

int vtkVJRenderer::UpdateGeometry()
{
  this->vtkOpenGLRenderer::UpdateGeometry();
}
