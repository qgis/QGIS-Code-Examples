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
//
//Qt related
//
#include "mainwindow.h"
#include "qtoolbar.h"

//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgssymbol.h>
#include <qgsvectorlayerlabeling.h>
#include <qgsfontutils.h>
#include <qgstextformat.h>
#include <qgspallabeling.h>
#include <qgsrasterlayer.h>
#include <qgscoordinatereferencesystem.h>

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

  if (mypLayer->isValid())
  {
    qDebug("Layer is valid");
  }
  else
  {
    qDebug("Layer is NOT valid");
    return;
  }
  
  //set up a renderer for the layer
  QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(QgsSymbol::defaultSymbol(mypLayer->geometryType()));
  QList <QgsMapLayer *> layers;
  mypLayer->setRenderer(mypRenderer);
  
  //
  //set up labelling for the layer
  //
  QgsPalLayerSettings labelSettings;
    labelSettings.fieldName = QStringLiteral( "NAME" );
  QgsTextFormat format;
  format.setFont( QgsFontUtils::getStandardTestFont( QStringLiteral( "Bold" ) ).family() );
  format.setSize( 12 );
  format.setNamedStyle( QStringLiteral( "Bold" ) );
  format.setColor( QColor( 200, 0, 200 ) );
  labelSettings.setFormat( format );

  //get the label instance associated with the layer
  mypLayer->setLabeling(new QgsVectorLayerSimpleLabeling(labelSettings));
  mypLayer->setLabelsEnabled(true);
  //and the label attributes associated with the label

  //get the field list associated with the layer
  //we'll print the names out to console for diagnostic purposes
  // QgsFieldMap myFields = mypLayer->dataProvider()->fields();
  // for (unsigned int i = 0; i < myFields.size(); i++ )
  // {
  //   qDebug("Field Name: " +  QString(myFields[i].name()).toLocal8Bit() );
  // }
  //just use the last field's name in the fields list as the label field! 
  // qDebug("set label field to " + QString(myFields[myFields.size()-1].name()).toLocal8Bit());
 
  /*
   * Here are a bunch of other things you can set based on values on a database field
   * the second parameter in each case would be the field name from which the 
   * attribute can be retrieved.
  mypLabel->setLabelField( QgsLabel::Family, "fontFamily" );
  mypLabel->setLabelField( QgsLabel::Bold,  "fontIsBold" );
  mypLabel->setLabelField( QgsLabel::Italic, "fontIsItalic"  );
  mypLabel->setLabelField( QgsLabel::Underline, "fontIsUnderlined"  );
  mypLabel->setLabelField( QgsLabel::Size, "fontSize" );
  mypLabel->setLabelField( QgsLabel::BufferSize,"fontBufferSize" );
  mypLabel->setLabelField( QgsLabel::XCoordinate, "labelX" );
  mypLabel->setLabelField( QgsLabel::YCoordinate, "labelY");
  mypLabel->setLabelField( QgsLabel::XOffset, "labelXOffset");
  mypLabel->setLabelField( QgsLabel::YOffset, "labelYOffset");
  mypLabel->setLabelField( QgsLabel::Alignment, "labelAlignment" );
  mypLabel->setLabelField( QgsLabel::Angle, "labelAngle");
  */
  
  //lastly we enable labelling!
  // mypLayer->enableLabels(true);
  
  // Add the Vector Layer to the Layer Registry
  QgsProject::instance()->addMapLayer(mypLayer, true);

  // Add the Layer to the Layer Set
  layers.append( mypLayer );

  QString url = "type=xyz&url=https://a.tile.openstreetmap.org/%7Bz%7D/%7Bx%7D/%7By%7D.png&zmax=19&zmin=0&zmin=0";
  // url.append("/%7Bz%7D/%7Bx%7D/%7By%7D.png&zmax=19&zmin=0&crs=EPSG3857");
  // QgsCoordinateReferenceSystem *crs = new QgsCoordinateReferenceSystem("EPSG:3857");
  mpMapCanvas->setDestinationCrs(* new QgsCoordinateReferenceSystem("EPSG:3857"));
  QgsRasterLayer *ras = new QgsRasterLayer(url, "OpenStreetMap", "wms");
  ras->setCrs(* new QgsCoordinateReferenceSystem("EPSG:3857"));
  // if (ras->isValid())
  // {
  //   qDebug("Raster layer is valid");
  // }
  // else
  // {
  //   qDebug("Raster layer is NOT valid");
  //   return;
  // }
  // layers.append( ras );
  // set teh canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());
  // Set the Map Canvas Layer Set
  mpMapCanvas->setLayers(layers);
}
