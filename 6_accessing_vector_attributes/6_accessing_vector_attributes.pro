TEMPLATE = app
TARGET = qgis_example6
QT = qt3support sql opengl network svg gui core xml
LANGUAGE= C++
linux-g++{
  QGISDIR=[path to installed qgis]
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=[path to qgis src directory]
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_core -lproj -lqgis_gui
}
macx{
  QGISDIR=/Users/timsutton/apps/qgis.app/Contents/MacOS/
  QGISLIBDIR=$${QGISDIR}/lib
  QGISSRCDIR=/Users/timsutton/dev/cpp/qgis/src/
  QGISPLUGINDIR=$${QGISLIBDIR}/qgis
  DEFINES += QGISPLUGINDIR=$${QGISPLUGINDIR}
  LIBS = -L$${QGISLIBDIR} -lqgis_core -lqgis_gui
  system(mkdir -p qgis_example3.app/Contents/MacOS/share/qgis/resources/)
  system(cp $${QGISDIR}/share/qgis/resources/srs.db qgis_example3.app/Contents/MacOS/share/qgis/resources/)
}

INCLUDEPATH = $${QGISDIR}/include/qgis \
              $${QGISSRCDIR}/core \
              $${QGISSRCDIR}/gui \
              $${QGISSRCDIR}/plugins \
              $${QGISSRCDIR}/providers \
              $${QGISSRCDIR}/raster \
              $${QGISSRCDIR}/ui 

CONFIG += qt gui exceptions stl warn_on debug thread

#RESOURCES += resources.qrc

FORMS += mainwindowbase.ui

HEADERS = mainwindow.h

SOURCES = main.cpp \
          mainwindow.cpp

