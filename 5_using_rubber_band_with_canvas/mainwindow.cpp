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
#include <QToolBar>
#include <QSvgRenderer>
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgsmapcanvas.h>
#include <qgscolorrampshader.h>
#include <qgsrastershader.h>
#include <qgssinglebandpseudocolorrenderer.h>//

// Needed fr rubber band support
//
#include <qgsrubberband.h>
#include <qgswkbtypes.h>
#include <qgspointxy.h>

//
// QGIS Map tools
//
#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags fl)
    : QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);

  // Instantiate Provider Registry
#if defined(Q_WS_MAC)
  QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
#else
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

  //create the rubber band
  QgsRubberBand *mpRubberBand = new QgsRubberBand(mpMapCanvas, QgsWkbTypes::PolygonGeometry );
  mpRubberBand->setVisible( true );
  qDebug("After show ruberband");
}

MainWindow::~MainWindow()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMapCanvas;
  delete mpLayout;
  delete mpRubberBand;
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
  QFileInfo myRasterFileInfo("../data/Abarema_jupunba_projection.tif");
  QgsRasterLayer * mypLayer = new QgsRasterLayer(myRasterFileInfo.filePath(), 
      myRasterFileInfo.completeBaseName());
  if (mypLayer->isValid())
  {
    qDebug("Layer is valid");
  }
  else
  {
    qDebug("Layer is NOT valid");
    return;
  }
  // render strategy for grayscale image (will be rendered as pseudocolor)
  mypLayer->setContrastEnhancement(
    QgsContrastEnhancement::StretchToMinimumMaximum);

  QgsRasterShader *rasterShader = new QgsRasterShader();
  QgsColorRampShader *colorRampShader = new QgsColorRampShader();
  colorRampShader->setColorRampType(QgsColorRampShader::Interpolated);

  QgsRasterBandStats stats = mypLayer->dataProvider()->bandStatistics(1);

  qDebug("Min value: %f", stats.minimumValue);
  qDebug("Max value: %f", stats.maximumValue);
  QList<QgsColorRampShader::ColorRampItem> colorRampItems;
  QgsColorRampShader::ColorRampItem firstItem;
  firstItem.value = stats.minimumValue;
  firstItem.label = printf("%f", stats.minimumValue);
  firstItem.color = QColor( 100, 149, 237 );
  colorRampItems.append( firstItem );


  QgsColorRampShader::ColorRampItem secondItem;
  secondItem.value = stats.maximumValue;
  secondItem.label = printf("%f", stats.maximumValue);
  secondItem.color = QColor( 165, 42, 42 );
  colorRampItems.append( secondItem );

  colorRampShader->setColorRampItemList( colorRampItems );
  rasterShader->setRasterShaderFunction( colorRampShader );

  QgsSingleBandPseudoColorRenderer *rasterRenderer = new QgsSingleBandPseudoColorRenderer( mypLayer->dataProvider(), 1, rasterShader );
  
  mypLayer->setRenderer( rasterRenderer );
  // mypLayer->setDrawingStyle( QgsRasterLayer::SingleBandPseudoColor );
  // mypLayer->setColorShadingAlgorithm( QgsRasterLayer::PseudoColorShader );
  // mypLayer->setContrastEnhancementAlgorithm(
  //   QgsContrastEnhancement::StretchToMinimumMaximum, false );
  // mypLayer->setMinimumValue( mypLayer->grayBandName(), 0.0, false );
  // mypLayer->setMaximumValue( mypLayer->grayBandName(), 10.0 );

  //create a layerset
  QList <QgsMapLayer *> layers;
  // Add the layers to the Layer Set
  layers.append(mypLayer);//bool visibility
  // set the canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());

  // Add the Vector Layer to the Layer Registry
  QgsProject::instance()->addMapLayer(mypLayer, true);

  // Set the Map Canvas Layer Set
  mpMapCanvas->setLayers(layers);
}
void MainWindow::on_mpToolShowRubberBand_clicked()
{
  qDebug("Begin show ruberband");
  QgsPointXY myPoint1 = mpMapCanvas->getCoordinateTransform()->toMapCoordinates(10.0, 10.0);
  qDebug("Intermediate 1 ruberband");
  mpRubberBand->addPoint(myPoint1);
  qDebug("Intermediate 2 ruberband");
  QgsPointXY myPoint2 = mpMapCanvas->getCoordinateTransform()->toMapCoordinates(20.0, 10.0);
  mpRubberBand->addPoint(myPoint2);
  QgsPointXY myPoint3 = mpMapCanvas->getCoordinateTransform()->toMapCoordinates(20.0, 20.0);
  mpRubberBand->addPoint(myPoint3);
  QgsPointXY myPoint4 = mpMapCanvas->getCoordinateTransform()->toMapCoordinates(10.0, 20.0);
  mpRubberBand->addPoint(myPoint4);
  QgsPointXY myPoint5 = mpMapCanvas->getCoordinateTransform()->toMapCoordinates(10.0, 10.0);
  mpRubberBand->addPoint(myPoint5);
  qDebug("End show ruberband");
}
void MainWindow::on_mpToolHideRubberBand_clicked()
{
  qDebug("Reset 1 Ruberband");
  mpRubberBand->reset(QgsWkbTypes::PolygonGeometry);
  qDebug("Reset 2 Ruberband");
  mpRubberBand->setVisible( false );
  qDebug("Reset 3 Ruberband");

}
