/***************************************************************************
 *   Copyright (C) 2006 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mainwindow.h"

//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgssymbol.h>
//
// QGIS Map tools
//
#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"
//
// These are the other headers for available map tools (not used in this example)
//
//#include "qgsmaptoolcapture.h"
//#include "qgsmaptoolidentify.h"
//#include "qgsmaptoolselect.h"
//#include "qgsmaptoolvertexedit.h"
//#include "qgsmeasure.h"
//

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags fl)
    : QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);

  // Instantiate Provider Registry
#if defined(Q_WS_MAC)
  //
  // Be sure to set this to an appropriate place if you are on OSX
  //
  QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
#else
  //
  // Be sure to set this to an appropriate place if you are on Linux or windows
  //
  QString myPluginsDir        = "/usr/lib/qgis";
#endif
  QgsProviderRegistry::instance(myPluginsDir);


  // Create the Map Canvas
  mpMapCanvas= new QgsMapCanvas();
  mpMapCanvas->enableAntiAliasing(true);
  mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mpMapCanvas->freeze(false);
  mpMapCanvas->setVisible(true);
  mpMapCanvas->refresh();
  mpMapCanvas->show();
  
  // Lay our widgets out in the main window
  mpLayout = new QVBoxLayout(frameMap);
  mpLayout->addWidget(mpMapCanvas);

  //create the action behaviours
  connect(mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
  connect(mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
  connect(mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
  connect(mpActionAddLayer, SIGNAL(triggered()), this, SLOT(addLayer()));

  //create a little toolbar
  mpMapToolBar = addToolBar(tr("File"));
  mpMapToolBar->addAction(mpActionAddLayer);
  mpMapToolBar->addAction(mpActionZoomIn);
  mpMapToolBar->addAction(mpActionZoomOut);
  mpMapToolBar->addAction(mpActionPan);

  //create the maptools
  mpPanTool = new QgsMapToolPan(mpMapCanvas);
  mpPanTool->setAction(mpActionPan);
  mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, false); // false = in
  mpZoomInTool->setAction(mpActionZoomIn);
  mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, true ); //true = out
  mpZoomOutTool->setAction(mpActionZoomOut);
}

MainWindow::~MainWindow()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMapCanvas;
  delete mpLayout;
}

void MainWindow::panMode()
{
  mpMapCanvas->setMapTool(mpPanTool);

}
void MainWindow::zoomInMode()
{
  mpMapCanvas->setMapTool(mpZoomInTool);
}
void MainWindow::zoomOutMode()
{
  mpMapCanvas->setMapTool(mpZoomOutTool);
}
void MainWindow::addLayer()
{
  QString myLayerPath         = "/home/thomasg/ne_10m_admin_0_countries.shp";
  QString myLayerBaseName     = "Countries";
  QString myProviderName      = "ogr";
  
  QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
  QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(QgsSymbol::defaultSymbol(mypLayer->geometryType()));
  QList <QgsMapLayer *> layers;
  mypLayer->setRenderer(mypRenderer);

  if (mypLayer->isValid())
  {
    qDebug("Layer is valid");
  }
  else
  {
    qDebug("Layer is NOT valid");
    return;
  }
  // Add the Vector Layer to the Project Registry
  QgsProject::instance()->addMapLayer(mypLayer, true);

  // Add the Layer to the Layer Set
  layers.append(mypLayer);
  // set the canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());
  // Set the Map Canvas Layers
  mpMapCanvas->setLayers(layers);
}
