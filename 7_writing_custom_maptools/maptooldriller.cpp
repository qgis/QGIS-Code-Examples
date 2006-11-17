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

#include "maptooldriller.h"
#include "qgsmapcanvas.h"
#include "qgsrasterlayer.h"
#include "qgsmaptopixel.h"
#include "qgscursors.h"
#include <QSettings>
#include <QMessageBox>
#include <QCursor>
#include <QPixmap>
#include <QHash>

MapToolDriller::MapToolDriller(QgsMapCanvas* canvas)
  : QObject(), QgsMapTool(canvas)
{
  // set cursor
  QPixmap myIdentifyQPixmap = QPixmap((const char **) identify_cursor);
  mCursor = QCursor(myIdentifyQPixmap, 1, 1);
}
    
MapToolDriller::~MapToolDriller()
{
}

void MapToolDriller::canvasMoveEvent(QMouseEvent * e)
{

}
  
void MapToolDriller::canvasPressEvent(QMouseEvent * e)
{
}

void MapToolDriller::canvasReleaseEvent(QMouseEvent * e)
{

  QgsPoint myPoint = mCanvas->getCoordinateTransform()->toMapCoordinates(e->x(), e->y());
  QHash <QString,double> myHash;
  myHash = drill(myPoint);
  emit drilled(myHash);
}

QHash<QString,double> MapToolDriller::drill(const QgsPoint &thePoint)
{
  QHash<QString,double> myHash;
  //ok so here is where the real work is done
  //we iterate through the layers in the map canvas
  //and check if each layer is a raster
  //if it is we add the looked up value for that raster to our 
  //hash table

  QgsMapLayer* layer = mCanvas->currentLayer();

  // call identify method for selected layer

  if (layer)
  {
      // convert screen coordinates to map coordinates

      if (layer->type() == QgsMapLayer::VECTOR)
      {
        //do nothing
      }
      else if (layer->type() == QgsMapLayer::RASTER)
      {
        if (dynamic_cast<QgsRasterLayer*>(layer)->providerKey() == "wms")
        {
          //do nothing
        }
        else
        {
          std::map<QString, QString> myAttributes;
          dynamic_cast<QgsRasterLayer*>(layer)->identify(thePoint, myAttributes);
        }
      }
      else
      {
#ifdef QGISDEBUG
        std::cout << "MapToolDriller::canvasReleaseEvent: unknown layer type!" << std::endl;
#endif
      }
  }
  return myHash;
}


void MapToolDriller::deactivate()
{
}
