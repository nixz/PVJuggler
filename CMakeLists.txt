#===========================================================================nix
# @file CMakeList.txt
# @author Nikhil Shetty <nikhil.j.shetty@gmail.com>
# @date Tue Mar 30 01:33:24 2010
#
# @brief To build the paraview/vrjuggler client.
#===========================================================================nix

cmake_minimum_required(VERSION 2.6)

PROJECT(ParaViewVRJugglerClient)

#---------------------------------------------------------------------functions
# find_libs
#   in: list of library names
#   out:list containing absolute path for each lib
function(find_libs liblist libpath outlist)
    foreach(libname ${liblist})
        find_library(LIBPATH_${libname} NAMES ${libname} PATHS ${libpath})
        if(NOT LIBPATH_${libname})
            message (SEND_ERROR "Library ${libname} not found.")
        else(NOT LIBPATH_${libname})
            list(APPEND loc_outlist ${LIBPATH_${libname}})
        endif(NOT LIBPATH_${libname})
    endforeach(libname)

    # message (STATUS "out list: ${loc_outlist}")
    set(${outlist} ${loc_outlist} PARENT_SCOPE)
endfunction(find_libs)

# generate installation rules
function(gen_install_rules target)
  install(TARGETS ${target}
            RUNTIME DESTINATION usr/bin
            LIBRARY DESTINATION usr/lib
            ARCHIVE DESTINATION usr/lib/static)
endfunction(gen_install_rules)

#-------------------------------------------------------------------------basic
# set(CMAKE_BUILD_TYPE  "RelWithDebInfo")
set(CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

SET(DEP_INCLUDE_DIRS)
# SET(DEP_LIBRARY_DIRS)
# SET(DEP_LIBRARIES)

#---------------------------------------------------------------------vrjuggler
find_package(VRJuggler 3.0 REQUIRED)

# find_package(Flagpoll REQUIRED)
# flagpoll_check_modules(VPR REQUIRED vpr)
# flagpoll_check_modules(GMTL REQUIRED gmtl)
# flagpoll_check_modules(GADGET REQUIRED gadgeteer)
# flagpoll_check_modules(VRJuggler REQUIRED vrjuggler)
# flagpoll_check_modules(VRJugglerOGL REQUIRED vrjuggler-opengl)

# find_libs("${VPR_LIBRARIES}"          "${VPR_LIBRARY_DIRS}"       VPR_libs)
# find_libs("${GADGET_LIBRARIES}"       "${GADGET_LIBRARY_DIRS}"    GADGET_libs)
# find_libs("${VRJuggler_LIBRARIES}"    "${VRJuggler_LIBRARY_DIRS}" VRJUGGLER_libs)
# find_libs("${VRJugglerOGL_LIBRARIES}" "${VRJugglerOGL_LIBRARY_DIRS}" VRJUGGLEROGL_libs)

LIST(APPEND DEP_INCLUDE_DIRS ${VPR_INCLUDE_DIRS})
LIST(APPEND DEP_INCLUDE_DIRS ${GMTL_INCLUDE_DIRS})
LIST(APPEND DEP_INCLUDE_DIRS ${GADGET_INCLUDE_DIRS})
LIST(APPEND DEP_INCLUDE_DIRS ${VRJUGGLER_INCLUDE_DIRS})
LIST(APPEND DEP_INCLUDE_DIRS ${VRJugglerOpenGL_INCLUDE_DIRS})

# LIST(APPEND DEP_LIBRARIES ${VPR_libs})
# LIST(APPEND DEP_LIBRARIES ${GADGET_libs})
# LIST(APPEND DEP_LIBRARIES ${VRJUGGLER_libs})
# LIST(APPEND DEP_LIBRARIES ${VRJUGGLEROGL_libs})

#----------------------------------------------------------------------Paraview
find_package(ParaView REQUIRED)
include(${PARAVIEW_USE_FILE})

message(${VRJUGGLER_INCLUDE_DIRS})
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
TARGET_LINK_LIBRARIES(VRJugglerClient vtkPVServerManager ${VRJUGGLER_LIBRARIES} ${BOOST_LIBRARIES})
INSTALL (TARGETS VRJugglerClient DESTINATION bin)
