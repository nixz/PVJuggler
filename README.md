PVJuggler
=========

- Its a collaborative ParaView client which uses VRJuggler for device and
  display management. Users of PVJuggler are people who want to run
  Visualizations on Virtual Environments (VE) like the CAVE, Tiled Wall etc and
  do some quick collaborative prototyping. So we assume here that users have a
  basic know how of the aforementioned tools.

- Currently begin developed and tested in Linux platforms only but other
  platforms should work as well. Any feedback or contribution from people trying
  it on other platforms is most welcome

- Users need to know basic aspects of ParaView (like setting up pipeline,
  connecting to a server etc) which is easy because it has a GUI and one can
  learn by clicking on different things. More difficult is the know-how of
  VRJuggler and running it in different environments so one has no choice but
  RTFM.

- This tools primarily conceived at University of Louisiana at Lafayette and
  being developed collaboration with Kiware Inc.

- People involved in this project are:

  Nikhil Shetty:
  - Phd student at University of Louisiana at Lafayette under Dr. Carolina Cruz-Neira.
  - Research Engineer at Kitware Inc.

  Sebastien Jourdain:
  - Research Engineer at Kitware Inc.

  Carolina Cruz-Neira:
  - W. Hansen Hall and Mary Officer Hall/BORSF Endowed Super Chair in
    Telecommunications at University of Louisiana at Lafayette.

Note: This is currently at prototyping stage.

Requirements:
------------

- ParaView 3.14
- VRJuggler 3.0
- CMake

Steps to Build and Install:
---------------------------
After cloning the repo

# mkdir ParaView-build
# cd ParaView-build
# cmake-gui ../ParaView

At this point VRJuggler and ParaView build/install locations may need to be
provided if not already found by CMake. Configure Generate and Exit the
cmake-gui. When everything looks okay run ..

# make
# make install ; Optional

Steps to Run:
-------------
Start paraview server in multi-client (collaboration mode)

[localhost] # pvserver --multi-clients

Start paraview and connect to pvserver at host (we are assuming local host)

[localhost] # paraview -url=cs://t:11111

Start PVJuggler client and connect to pvserver

[localhost] # VJ_BASE_DIR=<path/to/vrjuggler/base/directory> ./VRJugglerClient -vr=<path/to/vrjuggler/config/file/something.jconf>

