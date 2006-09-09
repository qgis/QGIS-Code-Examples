TEMPLATE = app
QT = qt3support sql opengl network svg gui core xml
LANGUAGE= C++
linux-g++{
  QGISDIR=[path to installed qgis]
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=[path to qgis src directory]
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_composer -lqgis_core -lproj -lqgis_gui  -lqgis_legend
}
macx{
  QGISDIR=/Users/timsutton/apps/qgis.app/Contents/MacOS/
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=/Users/timsutton/dev/cpp/qgis/src/
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_composer -lqgis_core -lqgis_gui  -lqgis_legend
}

INCLUDEPATH = $${QGISDIR}/include/qgis \
              $${QGISSRCDIR}/composer \
              $${QGISSRCDIR}/core \
              $${QGISSRCDIR}/designer \
              $${QGISSRCDIR}/gui \
              $${QGISSRCDIR}/helpviewer \
              $${QGISSRCDIR}/legend \
              $${QGISSRCDIR}/mac \
              $${QGISSRCDIR}/plugins \
              $${QGISSRCDIR}/providers \
              $${QGISSRCDIR}/raster \
              $${QGISSRCDIR}/ui \
              $${QGISSRCDIR}/widgets

CONFIG += qt gui exceptions stl warn_on debug thread

FORMS += mainwindowbase.ui

HEADERS = mainwindow.h

SOURCES = main.cpp \
          mainwindow.cpp

