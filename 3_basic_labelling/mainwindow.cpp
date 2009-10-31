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
#include <qgsvectordataprovider.h>
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
//Labelling related
//
#include <qgslabel.h>
#include <qgslabelattributes.h>
#include <qgsfield.h>

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
  mpMapCanvas->enableAntiAliasing(true);
  mpMapCanvas->useImageToRender(false);
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
  mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
  mpZoomInTool->setAction(mpActionZoomIn);
  mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
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
  QString myLayerPath         = "../data";
  QString myLayerBaseName     = "test";
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
  QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
  QList<QgsMapCanvasLayer> myLayerSet;
  mypLayer->setRenderer(mypRenderer);
  
  //
  //set up labelling for the layer
  //

  //get the label instance associated with the layer
  QgsLabel * mypLabel;
  mypLabel = mypLayer->label();
  //and the label attributes associated with the label
  QgsLabelAttributes * mypLabelAttributes;
  mypLabelAttributes = mypLabel->layerAttributes();
  //note in QGIS 1.4 and up you should use mypLabel->labelAttributes rather

  //get the field list associated with the layer
  //we'll print the names out to console for diagnostic purposes
  QgsFieldMap myFields = mypLayer->dataProvider()->fields();
  for (unsigned int i = 0; i < myFields.size(); i++ )
  {
    qDebug("Field Name: " +  QString(myFields[i].name()).toLocal8Bit() );
  }
  //just use the last field's name in the fields list as the label field! 
  qDebug("set label field to " + QString(myFields[myFields.size()-1].name()).toLocal8Bit());
  mypLabel->setLabelField( QgsLabel::Text,  myFields.size()-1);
  //set the colour of the label text
  mypLabelAttributes->setColor(Qt::black);
  //create a 'halo' effect around each label so it
  //can still be read on dark backgrounds
  mypLabelAttributes->setBufferEnabled(true);
  mypLabelAttributes->setBufferColor(Qt::yellow);
  int myType = QgsLabelAttributes::PointUnits;
  mypLabelAttributes->setBufferSize(1,myType);
  
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
  mypLayer->enableLabels(true);
  
  // Add the Vector Layer to the Layer Registry
  QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);

  // Add the Layer to the Layer Set
  myLayerSet.append(QgsMapCanvasLayer( mypLayer ) );
  // set teh canvas to the extent of our layer
  mpMapCanvas->setExtent(mypLayer->extent());
  // Set the Map Canvas Layer Set
  mpMapCanvas->setLayerSet(myLayerSet);
}

