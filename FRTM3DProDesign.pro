QT       += opengl core gui

TARGET = FRTM3DProDesign
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

SOURCES += src/main.cpp \
    src/mainview.cpp \
    src/view.cpp \
    src/model.cpp \
    src/cameraviewfrustum.cpp \
    src/cameras.cpp \
    src/cameraview.cpp \
    src/glslprogram.cpp \
    src/glslmainviewprogram.cpp \
    src/glslfrustumprogram.cpp \
    src/glslzbufferprogram.cpp \
    src/objloader.cpp \
    src/cameraszbuffer.cpp \
    src/camera3dmodel.cpp \
    src/cameralabels.cpp \
    src/camerasmemento.cpp \
    src/mainwindow.cpp \
    src/glsl3dcameraprogram.cpp \
    src/glslcameraviewprogram.cpp \
    src/camera3dmodel.cpp \
    src/cameralabels.cpp \
    src/cameras.cpp \
    src/camerasmemento.cpp \
    src/cameraszbuffer.cpp \
    src/cameraview.cpp \
    src/cameraviewfrustum.cpp \
    src/glsl3dcameraprogram.cpp \
    src/glslcameraviewprogram.cpp \
    src/glslfrustumprogram.cpp \
    src/glslmainviewprogram.cpp \
    src/glslprogram.cpp \
    src/glslzbufferprogram.cpp \
    src/mainview.cpp \
    src/mainwindow.cpp \
    src/model.cpp \
    src/objloader.cpp \
    src/view.cpp \
    src/frtmutils.cpp

HEADERS += \
    src/view.h \
    src/model.h \
    src/mainview.h \
    src/cameraviewfrustum.h \
    src/cameras.h \
    src/cameraview.h \
    src/camerasmemento.h \
    src/glslprogram.h \
    src/glslmainviewprogram.h \
    src/glslfrustumprogram.h \
    src/glslzbufferprogram.h \
    src/objloader.h \
    src/cameraszbuffer.h \
    src/camera3dmodel.h \
    src/cameralabels.h \
    src/mainwindow.h \
    src/glsl3dcameraprogram.h \
    src/glslcameraviewprogram.h \
    src/frtmutils.h

GLM_INCLUDE_PATH = "$$PWD/3rdParty/glm"
GLEW_INCLUDE_PATH = "$$PWD/3rdParty/glew-1.13.0/include"
GLEW_LIB_PATH = "$$PWD/3rdParty/glew-1.13.0/lib/Release/x64"

INCLUDEPATH  += $$GLM_INCLUDE_PATH $$GLEW_INCLUDE_PATH
LIBS += -L$$GLEW_LIB_PATH -lglew32
LIBS += -lOpenGL32

GLEW_DLL.files = "$$PWD/3rdParty/glew-1.13.0/bin/Release/x64/glew32.dll"
GLEW_DLL.path= "$$PWD"

INSTALLS += GLEW_DLL

RESOURCES += \
    frtm3dprodesign.qrc
