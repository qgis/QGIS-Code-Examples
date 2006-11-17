/***************************************************************************
    qgsmaptooldriller.h  -  This tool will drill through a bunch of rasters
                            and produce a table of values at a given point
                            in each layer
    ---------------------
    begin                : November 2006
    copyright            : (C) 2006 by Tim Sutton
    email                : tim at linfiniti.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* $Id$ */

#ifndef MAPTOOLDRILLER_H
#define MAPTOOLDRILLER_H

#include "qgsmaptool.h"
#include "qgspoint.h"

#include <QObject>
#include <QHash>
#include <QString>

class QgsMapLayer;
class QgsRasterLayer;
class QgsVectorLayer;

/**
  \brief Map tool for drilling down through several raster layers and
         returning a hash of the results.
*/
class MapToolDriller : public QObject, public QgsMapTool
{
  Q_OBJECT;

  public:
    MapToolDriller(QgsMapCanvas* canvas);

    ~MapToolDriller();

    //! Overridden mouse move event
    virtual void canvasMoveEvent(QMouseEvent * e);

    //! Overridden mouse press event
    virtual void canvasPressEvent(QMouseEvent * e);

    //! Overridden mouse release event
    virtual void canvasReleaseEvent(QMouseEvent * e);

    //! called when map tool is being deactivated
    virtual void deactivate();

    typedef QHash<QString,double> DrillResult;
  signals:
    void drilled(DrillResult);



  private:
    QHash<QString,double>  drill(const QgsPoint& myPoint);
private slots:


};

#endif
