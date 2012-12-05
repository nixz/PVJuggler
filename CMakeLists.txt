#===========================================================================nix
# @file CMakeList.txt
# @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
# @date Tue Mar 30 01:33:24 2010
#
# @brief To build the paraview/vrjuggler client.
#===========================================================================nix

cmake_minimum_required(VERSION 2.6)

PROJECT(ParaViewVRJugglerClient)

#-----------------------------------------------------------------------------
find_package(VRJuggler 3.0 REQUIRED)
find_package(ParaView REQUIRED)
include(${PARAVIEW_USE_FILE})

#-------------------------------------------------------------------Application
INCLUDE_DIRECTORIES(
  ${VRJUGGLER_INCLUDE_DIRS}
  "${${PROJECT_NAME}_SOURCE_DIR}"
  "${${PROJECT_NAME}_SOURCE_DIR}/Navigation")

ADD_EXECUTABLE(VRJugglerClient
  # main.cpp
  # MPApp.cpp
  # Mesh.h
  main.cxx
  vtkVRJugglerGUIHelper.cxx
  vtkVRJugglerOptions.cxx
  vtkVRJugglerRenderer.cxx
#   vtkVJRenderer.h
  vtkVJRenderer.cxx
  # Navigation/GamepadNavigation.cpp
  # Navigation/KeyBoardMouseNavigation.cpp
  # Navigation/NavigationBase.cpp
  # Navigation/vtkVRJNavigation.cpp
  )
TARGET_LINK_LIBRARIES( VRJugglerClient
  vtkPVServerManagerApplication
  vtkPVServerManagerDefault
  vtkPVClientServerCoreDefault
  vtkPVVTKExtensionsDefault
  ${VRJUGGLER_LIBRARIES})

INSTALL (TARGETS VRJugglerClient DESTINATION bin)
