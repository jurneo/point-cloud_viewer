#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T11:27:39
#
#-------------------------------------------------

QT       += core widgets gui concurrent

TARGET = pcl-test
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += _USE_MATH_DEFINES FLANN_STATIC

TEMPLATE = app

PCL_ROOT = $$quote($$(PCL_DIR))
WORKSPACE = $$quote($$(SISWORKSPACE))

INCLUDEPATH += ./ui ./thirdparty/viicp ./thirdparty/sparseicp \
               $$(BOOSTDIR) \
               $$(SISWORKSPACE)/platform/include \
               $$(SISWORKSPACE)/libraries/glut \
               $$(SISWORKSPACE)/framework/pcl/build/include \
               $$(SISWORKSPACE)/framework/pcl/io/include \
               $$(SISWORKSPACE)/framework/pcl/registration/include \
               $$(SISWORKSPACE)/framework/pcl/common/include \
               $$(SISWORKSPACE)/framework/pcl/sample_consensus/include \
               $$(SISWORKSPACE)/framework/pcl/search/include \
               $$(SISWORKSPACE)/framework/pcl/kdtree/include \
               $$(SISWORKSPACE)/framework/pcl/features/include \
               $$(SISWORKSPACE)/framework/pcl/octree/include \
               $$(SISWORKSPACE)/framework/pcl/filters/include \
               $$(SISWORKSPACE)/framework/pcl/segmentation/include \
               $$(SISWORKSPACE)/libraries/flann/include \
               $$(SISWORKSPACE)/libraries/eigen

LIBS += \


Debug:LIBS += -L$$(BOOSTDIR)/lib \
        -L$$(SISWORKSPACE)/libraries/glut -lglut64 \
        -L$$(SISWORKSPACE)/platform/lib -lmodeld -lgeometryd -lpointcloudd -lutild -liod \
        -L$$(SISWORKSPACE)/libraries/pcl/lib -lpcl_kdtree_debug -lpcl_common_debug -lpcl_filters_debug -lpcl_segmentation_debug -lpcl_features_debug -lpcl_io_debug -lpcl_search_debug -lpcl_registration_debug

Release:LIBS += -L$$(BOOSTDIR)/lib \
        -L$$(SISWORKSPACE)/libraries/glut -lglut64 \
        -L$$(SISWORKSPACE)/libraries/flann/lib -lflann \
        -L$$(SISWORKSPACE)/platform/lib -lmodel -lgeometry -lpointcloud -lutil -lio \
        -L$$(SISWORKSPACE)/libraries/pcl/lib -lpcl_kdtree_release -lpcl_common_release -lpcl_filters_release -lpcl_segmentation_release -lpcl_features_release -lpcl_io_release -lpcl_search_release -lpcl_registration_release


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
