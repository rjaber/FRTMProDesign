FRTMProDesigner
===============

3D Surveillance Designer

- 3D rendering developed with modern OpenGL including vertex & fragment shaders
- Makes use of projective texture mapping
- Developed software using OOP principles and design patterns
- Doxygen documentation: http://jaybird19.github.io/FRTMProDesign/
- Demo video: https://www.youtube.com/watch?v=G1GyezFv3XE
- Technologies: Qt, C++11, STL, OpenGL, GLSL, GLM

OS
==
- Developed on/for Windows 7 & Windows 10

Dependencies
============
- Requires Qt5 (Open Source and freely available http://www.qt.io/)

Building Instructions
=====================
```bash
git clone https://github.com/jaybird19/FRTMProDesign.git
cd FRTMProDesign
qmake -r
nmake
```

- To generate a Visual Studio Solution:
```bash
qmake -r -tp vc
```
