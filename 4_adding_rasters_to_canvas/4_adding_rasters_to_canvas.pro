TEMPLATE = app
TARGET = qgis_example4
QT = qt3support sql network svg gui core xml
LANGUAGE= C++
linux-g++{
  QGISDIR=[path to installed qgis]
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=[path to qgis src directory]
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_core -lqgis_gui -lproj 
}
macx{
  QGISDIR=/Applications/qgis.app/Contents/MacOS/
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=/Users/timsutton/dev/cpp/qgis/src/
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_core -lqgis_gui \
         -L/Library/Frameworks/GDAL.framework/unix/lib/
  INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers
}

INCLUDEPATH += $${QGISDIR}/include/qgis #\
              #$${QGISSRCDIR}/core \
              #$${QGISSRCDIR}/gui \
              #$${QGISSRCDIR}/plugins \
              #$${QGISSRCDIR}/providers \
              #$${QGISSRCDIR}/raster \
              #$${QGISSRCDIR}/ui 

DEFINES += CORE_EXPORT=""
DEFINES += GUI_EXPORT=""

CONFIG += qt gui exceptions stl warn_on debug thread

RESOURCES += resources.qrc

FORMS += mainwindowbase.ui

HEADERS = mainwindow.h

SOURCES = main.cpp \
          mainwindow.cpp

