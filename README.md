FRTMProDesigner
===============
Usage
=====

ScreenShots
===========
Main Window (Drag and Drop Cameras)
![Alt text](./misc/screenshots/main_window.jpg?raw=true "Main Window")

Enhanced Camera View
![Alt text](./misc/screenshots/room_corner.jpg?raw=true "Enahnced Camera View")


3D Surveillance Designer
========================

- 3D rendering developed with modern OpenGL including vertex & fragment shaders
- Makes use of projective texture mapping
- Developed software using OOP principles and design patterns
- Doxygen documentation: http://jaybird19.github.io/FRTMProDesign/
- Demo video: https://www.youtube.com/watch?v=G1GyezFv3XE
- Technologies: Qt, C++11, STL, OpenGL, GLSL, GLM

Platform
========
- Developed on/for Windows 7 (x86-64) & Windows 10 (x86-64)

Dependencies
============
- Requires Qt5 (Open Source and freely available http://www.qt.io/)
- nmake or jom for compiling

Building Instructions
=====================
```bash
git clone https://github.com/jaybird19/FRTMProDesign.git
cd FRTMProDesign
qmake -r
nmake release
nmake debug
nmake install
```

- To generate a Visual Studio Solution:
```bash
qmake -r -tp vc
```
