cmake_minimum_required(VERSION 2.6)
PROJECT(timtut2)
SET(CMAKE_COLOR_MAKEFILE ON)
# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules ${CMAKE_MODULE_PATH})
FIND_PACKAGE(QGIS REQUIRED)
IF (NOT QGIS_FOUND)
  MESSAGE (SEND_ERROR "QGIS dependency was not found!")
ENDIF (NOT QGIS_FOUND)
IF (WIN32)
  # expect that classes are being imported by default
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  # import qgis classes
  IF (MSVC)
    ADD_DEFINITIONS("-DGUI_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DCORE_EXPORT=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("\"-DGUI_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DCORE_EXPORT=__declspec(dllimport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DGUI_EXPORT=)
  ADD_DEFINITIONS(-DCORE_EXPORT=)
ENDIF (WIN32)

FIND_PACKAGE(GDAL REQUIRED)
IF (NOT GDAL_FOUND)
  MESSAGE (SEND_ERROR "GDAL dependency was not found!")
ENDIF (NOT GDAL_FOUND)

FIND_PACKAGE(GEOS REQUIRED)
IF (NOT GEOS_FOUND)
  MESSAGE (SEND_ERROR "GEOS dependency was not found!")
ENDIF (NOT GEOS_FOUND)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-Dtimtut2DEBUG=1)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
########################################################
# Files

SET (timtut2_SRCS
     main.cpp
     mainwindow.cpp
)

# This tut uses no UIs
SET (timtut2_UIS 
  mainwindowbase.ui
)

# This tut needs no MOC
SET (timtut2_MOC_HDRS
  mainwindow.h
)


SET (timtut2_RCCS
 resources.qrc 
)

SET (QT_USE_QT3SUPPORT FALSE)
SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTSVG TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTNETWORK TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )
########################################################
# Build

QT4_WRAP_UI (timtut2_UIS_H  ${timtut2_UIS})

QT4_WRAP_CPP (timtut2_MOC_SRCS  ${timtut2_MOC_HDRS})

QT4_ADD_RESOURCES(timtut2_RCC_SRCS ${timtut2_RCCS})

ADD_EXECUTABLE (timtut2 ${timtut2_SRCS} ${timtut2_MOC_SRCS} ${timtut2_RCC_SRCS} ${timtut2_UIS_H})

INCLUDE_DIRECTORIES(
     ${GDAL_INCLUDE_DIR}
     ${GEOS_INCLUDE_DIR}
     ${CMAKE_CURRENT_BINARY_DIR}
     ${QT_INCLUDE_DIR}
     ${QGIS_INCLUDE_DIR}
     .
)

#This is probably no longer needed, but I will leave it in for Win machines for the moment
IF(QT_QTSQL_FOUND)
 FIND_LIBRARY(QT_QTSQL_LIBRARY NAMES QtSql QtSql4 PATHS ${QT_LIBRARY_DIR} NO_DEFAULT_PATH)
 SET(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QTSQL_LIBRARY})
ENDIF(QT_QTSQL_FOUND)

TARGET_LINK_LIBRARIES(timtut2
  ${QT_LIBRARIES}
  ${QGIS_CORE_LIBRARY} 
  ${QGIS_GUI_LIBRARY}
  ${GEOS_LIBRARY}
  ${GDAL_LIBRARY}
)

IF (MSVC)
  # Very important or you get all kinds of odd side
  #effects like app crash on start up saying qtgui.dll
  TARGET_LINK_LIBRARIES( timtut2
                         qtmain
                       )
ENDIF (MSVC)

########################################################
# Install


