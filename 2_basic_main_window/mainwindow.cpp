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
#include <QAction>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QString>
#include <QToolBar>
#include <QFrame>
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
// Std Includes
//
#include <deque.h>
#include <iostream>

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);
#if defined(Q_WS_MAC)
  QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
  QString myLayerPath         = "/Users/timsutton/gisdata/brazil/BR_Cidades/";
  QString myLayerBaseName     = "Brasil_Cap";
  QString myProviderName      = "ogr";
#else
  QString myPluginsDir        = "/home/timlinux/apps/lib/qgis";
  QString myLayerPath         = "/home/timlinux/gisdata/brazil/BR_Cidades/";
  QString myLayerBaseName     = "Brasil_Cap";
  QString myProviderName      = "ogr";
#endif

  // Instantiate Provider Registry
  QgsProviderRegistry::instance(myPluginsDir);

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

  // Create the Map Canvas
  QgsMapCanvas * mypMapCanvas= new QgsMapCanvas(0, 0);
  mypMapCanvas->setExtent(mypLayer->extent());
  qDebug(mypMapCanvas->extent().stringRep(2));
  mypMapCanvas->enableAntiAliasing(true);
  mypMapCanvas->useQImageToRender(false);
  mypMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mypMapCanvas->freeze(false);
  mypMapCanvas->setVisible(true);
  mypMapCanvas->refresh();

  // Set the Map Canvas Layer Set
  mypMapCanvas->setLayerSet(myLayerSet);
  mypMapCanvas->show();
  // Lay our widgets out in the main window
  QVBoxLayout *  mypLayout = new QVBoxLayout(frameMap);
  mypLayout->addWidget(mypMapCanvas);
}

MainWindow::~MainWindow()
{

}
