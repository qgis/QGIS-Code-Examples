TEMPLATE = lib
CONFIG = qt
QT += xml qt3support
unix:LIBS += -lqgis_core -lqgis_gui -lqgis_raster -lqgis_legend -lproj
INCLUDEPATH += /usr/include/qgis
SOURCES = qgspointconverterplugin.cpp
HEADERS = qgspointconverterplugin.h
DEST = pointconverterplugin.so
