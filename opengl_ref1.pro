#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T11:27:39
#
#-------------------------------------------------

QT       += core widgets gui concurrent

TARGET = pcl_viewer
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += _USE_MATH_DEFINES FLANN_STATIC

TEMPLATE = app

PCL_ROOT = $$quote($$(PCL_DIR))
WORKSPACE = $$quote($$(SISWORKSPACE))

INCLUDEPATH += ./ui \
               $$(BOOSTDIR) \
               $$(SISWORKSPACE)/platform/include \
               $$(SISWORKSPACE)/libraries/glut

LIBS += -L$$(BOOSTDIR)/lib \
        -L$$(SISWORKSPACE)/libraries/glut -lglut64

SOURCES += main.cpp \
    testwindow.cpp \
    ui/qtgraphicsview.cpp \
    glmesh.cpp \
    utility.cpp

HEADERS += \
    testwindow.h \
    ui/qtgraphicsview.h \
    glmesh.h \
    utility.h

FORMS += \
    testwindow.ui

RESOURCES += \
    resource.qrc
