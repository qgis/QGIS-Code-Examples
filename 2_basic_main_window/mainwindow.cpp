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
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
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
// Std Includes
//
#include <deque.h>

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);

  // Instantiate Provider Registry
#if defined(Q_WS_MAC)
  QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
#else
  QString myPluginsDir        = "/home/timlinux/apps/lib/qgis";
#endif
  QgsProviderRegistry::instance(myPluginsDir);


  // Create the Map Canvas
  mpMapCanvas= new QgsMapCanvas(0, 0);
  qDebug(mpMapCanvas->extent().stringRep(2));
  mpMapCanvas->enableAntiAliasing(true);
  mpMapCanvas->useQImageToRender(false);
  mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mpMapCanvas->freeze(false);
  mpMapCanvas->setVisible(true);
  mpMapCanvas->refresh();
  mpMapCanvas->show();
  
  // Lay our widgets out in the main window
  mpLayout = new QVBoxLayout(frameMap);
  mpLayout->addWidget(mpMapCanvas);

  //create the action behaviours
  connect(mActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
  connect(mActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
  connect(mActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
  connect(mActionAddLayer, SIGNAL(triggered()), this, SLOT(addLayer()));

  //create a little toolbar
  mpMapToolBar = addToolBar(tr("File"));
  mpMapToolBar->addAction(mActionAddLayer);
  mpMapToolBar->addAction(mActionZoomIn);
  mpMapToolBar->addAction(mActionZoomOut);
  mpMapToolBar->addAction(mActionPan);

}

MainWindow::~MainWindow()
{

}

void MainWindow::panMode()
{
  QgsMapTool* mypTool = new QgsMapToolPan(mpMapCanvas);
  mypTool->setAction(mActionPan);
  mpMapCanvas->setMapTool(mypTool);

}
void MainWindow::zoomInMode()
{
  QgsMapTool* mypTool = new QgsMapToolZoom(mpMapCanvas, FALSE);
  mypTool->setAction(mActionZoomIn);
  mpMapCanvas->setMapTool(mypTool);
}
void MainWindow::zoomOutMode()
{
  QgsMapTool* mypTool = new QgsMapToolZoom(mpMapCanvas, FALSE);
  mypTool->setAction(mActionZoomOut);
  mpMapCanvas->setMapTool(mypTool);
}
void MainWindow::addLayer()
{
  QString myLayerPath         = "data";
  QString myLayerBaseName     = "test";
  QString myProviderName      = "ogr";
  
  QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
  QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->vectorType());
  std::deque<QString> myLayerSet;
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
  // Add the Vector Layer to the Layer Registry
  QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);

  // Add the Layer to the Layer Set
  myLayerSet.push_back(mypLayer->getLayerID());
  mypLayer->setVisible(TRUE);
  // set teh canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());
  // Set the Map Canvas Layer Set
  mpMapCanvas->setLayerSet(myLayerSet);
}

