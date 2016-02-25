QT       += opengl core gui

TARGET = FRTM3DProDesign
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11
CONFIG	 += debug_and_release

CONFIG(debug, debug|release) {
    DESTDIR = debug 
}

CONFIG(release, debug|release) {
    DESTDIR = release 
}

TEMPLATE = app

SOURCES = src/main.cpp \
    src/mainview.cpp \
    src/view.cpp \
    src/cameraviewfrustum.cpp \
    src/glslprogram.cpp \
    src/glslmainviewprogram.cpp \
    src/glslfrustumprogram.cpp \
    src/glslzbufferprogram.cpp \
    src/cameraszbuffer.cpp \
    src/camerasmemento.cpp \
    src/mainwindow.cpp \
    src/glsl3dcameraprogram.cpp \
    src/glslcameraviewprogram.cpp \
    src/camera3dmodel.cpp \
    src/cameralabels.cpp \
    src/cameras.cpp \
    src/cameraview.cpp \
    src/model.cpp \
    src/objloader.cpp \
    src/frtmutils.cpp

HEADERS = \
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
INCLUDEPATH += $$GLM_INCLUDE_PATH 

win32 {
	INCLUDEPATH += $$GLEW_INCLUDE_PATH
	GLEW_INCLUDE_PATH = "$$PWD/3rdParty/win32/glew-1.13.0/include"
	GLEW_LIB_PATH = "$$PWD/3rdParty/win32/glew-1.13.0/lib/Release/x64"
	LIBS += -L$$GLEW_LIB_PATH -lglew32
	LIBS += -lOpenGL32

	GLEW_DLL_DEBUG.files = "$$PWD/3rdParty/win32/glew-1.13.0/bin/Release/x64/glew32.dll"
	GLEW_DLL_DEBUG.path= "$$PWD/debug"

	GLEW_DLL_RELEASE.files = "$$PWD/3rdParty/win32/glew-1.13.0/bin/Release/x64/glew32.dll"
	GLEW_DLL_RELEASE.path= "$$PWD/release"

	INSTALLS += GLEW_DLL_DEBUG GLEW_DLL_RELEASE
}

unix {
	LIBS += -lGLEW -lGL
}

RESOURCES += \
    frtm3dprodesign.qrc
