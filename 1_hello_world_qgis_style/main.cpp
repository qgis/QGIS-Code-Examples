//
// Qt Includes
//
#include <QString>
#include <QApplication>
#include <QWidget>
#include <QJsonObject>
#include <QDomDocument>
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgssymbol.h>
#include <qgsmapcanvas.h>


int main(int argc, char ** argv) 
{
  // Start the Application
  QgsApplication app(argc, argv, true);

  QString myPluginsDir        = "/usr/lib/qgis/";
  QString myLayerPath         = "/home/thomasg/ne_10m_admin_0_countries.shp";
  QString myLayerBaseName     = "Countries";
  QString myProviderName      = "ogr";
  // Instantiate Provider Registry
  QgsProviderRegistry::instance(myPluginsDir);
  // create a maplayer instance
  QgsVectorLayer * mypLayer = 
      new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
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
  }

  // Add the Vector Layer to the Layer Registry
  QgsProject::instance()->addMapLayer(mypLayer);
  // Add the Layer to the Layer Set
  layers.append(mypLayer);

  // Create the Map Canvas
  QgsMapCanvas * mypMapCanvas = new QgsMapCanvas();
  mypMapCanvas->setExtent(mypLayer->extent());
  mypMapCanvas->enableAntiAliasing(true);
  mypMapCanvas->setCanvasColor(QColor(255, 255, 255));
  mypMapCanvas->freeze(false);
  // Set the Map Canvas Layer Set
  mypMapCanvas->setLayers(layers);
  mypMapCanvas->setVisible(true);
  mypMapCanvas->refresh();

  // Start the Application Event Loop
  return app.exec();
}
