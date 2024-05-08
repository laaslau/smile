# smile
opencv + dear imagui

Simple demonstration of using opnecv libraries and Dear ImGui.
It will open camera (or any other) stream, apply smile detection in face region (if such recognized). 
The haarcascade_frontalface_default.xml and  haarcascade_smile.xml used (both are part of opencv installation).
The result of processing shown in separate window and also in the one where original vidoe run.
The smile detection result shown in result window (simile or not so smile icon).

# build
**preconditions:**
- windows os
- opencv libraries.
- cmake ver 3.20 and up
- Visual Studio 17 2022

Use standard build usnng CMAKE.
**NB!!! It neccessary to update set(OpenCV_DIR path to opencv installation folder) in CMakeLists in cvlib folder.**
Also cmake --install . needed to copy the neccesary files to running folder 

# solution & projects
Solution divided into projects:
- **cvbench** (main projetc), contains main function. Creates and unites different logical units.
- **cvlib** wrapper to opencv. Contains all frame processing modules. Access to functionality over factory functions that will provide objects over interface abstractions (factory.cpp)
- **gui** implementation of graphic interface. Contains so called windows, container that keeps track, directX 11 wrapper (the one used for rendering)
- **imgui** snippet of Dear ImGui. Only neccessary files extracted and used.
- **common** definitions of system-wide interfaces.
- **toolbox** commonly used code.

  In addition PLOG header based logger used.



  
