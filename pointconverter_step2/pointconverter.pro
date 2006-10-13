#base directory of the qgis installation
QGIS_DIR = /home/humarco/src/qgis

TEMPLATE = lib
CONFIG = qt
QT += xml qt3support
unix:LIBS += -L/$$QGIS_DIR/lib -lqgis_core -lqgis_gui -lqgis_raster -lqgis_legend -lproj
INCLUDEPATH += $$QGIS_DIR/src/ui $$QGIS_DIR/src/plugins  $$QGIS_DIR/src/gui $$QGIS_DIR/src/raster $$QGIS_DIR/src/core $$QGIS_DIR 
SOURCES = qgspointconverterplugin.cpp
HEADERS = qgspointconverterplugin.h
DEST = pointconverterplugin.so
