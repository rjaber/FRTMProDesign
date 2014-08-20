#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T20:36:17
#
#-------------------------------------------------

QT       += opengl core

QT       += gui

TARGET = FRTM3DProDesign
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

SOURCES += main.cpp \
    mainview.cpp \
    view.cpp \
    model.cpp \
    cameraviewfrustum.cpp \
    cameras.cpp \
    cameraview.cpp \
    glslprogram.cpp \
    glslmainviewprogram.cpp \
    glslfrustumprogram.cpp \
    glslzbufferprogram.cpp \
    objloader.cpp \
    cameraszbuffer.cpp \
    camera3dmodel.cpp \
    cameralabels.cpp \
    camerasmemento.cpp \
    mainwindow.cpp \
    glsl3dcameraprogram.cpp \
    glslcameraviewprogram.cpp \
    camera3dmodel.cpp \
    cameralabels.cpp \
    cameras.cpp \
    camerasmemento.cpp \
    cameraszbuffer.cpp \
    cameraview.cpp \
    cameraviewfrustum.cpp \
    glsl3dcameraprogram.cpp \
    glslcameraviewprogram.cpp \
    glslfrustumprogram.cpp \
    glslmainviewprogram.cpp \
    glslprogram.cpp \
    glslzbufferprogram.cpp \
    mainview.cpp \
    mainwindow.cpp \
    model.cpp \
    objloader.cpp \
    view.cpp

GLM_INCLUDE_PATH = "C:/OpenGL/include"
GLEW_INCLUDE_PATH = "C:/OpenGL/include"
GLEW_LIB_PATH = "C:/OpenGL/lib"

INCLUDEPATH  += $$GLM_INCLUDE_PATH $$GLEW_INCLUDE_PATH
LIBS += -L$$GLEW_LIB_PATH -lglew32

INCLUDEPATH += C:\\OpenCV-2.4.8\\opencv\\build\\include
LIBS += -LC:\\OpenCV-2.4.8\\opencv\\build\\x64\\vc11\\lib \
    -lopencv_calib3d248d \
    -lopencv_contrib248d \
    -lopencv_core248d \
    -lopencv_features2d248d \
    -lopencv_flann248d \
    -lopencv_gpu248d \
    -lopencv_highgui248d \
    -lopencv_imgproc248d \
    -lopencv_legacy248d \
    -lopencv_ml248d \
    -lopencv_objdetect248d \
    -lopencv_ts248d \
    -lopencv_video248d \

HEADERS += \
    view.h \
    model.h \
    mainview.h \
    cameraviewfrustum.h \
    cameras.h \
    cameraview.h \
    camerasmemento.h \
    glslprogram.h \
    glslmainviewprogram.h \
    glslfrustumprogram.h \
    glslzbufferprogram.h \
    objloader.h \
    cameraszbuffer.h \
    camera3dmodel.h \
    cameralabels.h \
    glrenderutils.h \
    mainwindow.h \
    glsl3dcameraprogram.h \
    glslcameraviewprogram.h

FORMS +=

RESOURCES += \
    frtm3dprodesign.qrc
