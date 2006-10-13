#include "qgspointconverterplugin.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include <QDomNode>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#ifdef WIN32
#define QGISEXTERN extern "C" __declspec( dllexport )
#else
#define QGISEXTERN extern "C"
#endif

QgsPointConverterPlugin::QgsPointConverterPlugin(QgisApp* app, QgisIface* iface): mApp(app), mIface(iface), mAction(0)
{

}

QgsPointConverterPlugin::~QgsPointConverterPlugin()
{

}

void QgsPointConverterPlugin::initGui()
{
  mAction = new QAction(tr("&Convert to point"), this);
  connect(mAction, SIGNAL(activated()), this, SLOT(convertToPoint()));
  // Add the action to toolbar and the plugin menu
  mIface->addToolBarIcon(mAction);
  mIface->addPluginMenu(tr("&Convert to point"), mAction);
}

void QgsPointConverterPlugin::unload()
{
  mIface->removeToolBarIcon(mAction);
  mIface->removePluginMenu(tr("&Convert to point"), mAction);
  delete mAction;
}

void QgsPointConverterPlugin::convertToPoint()
{
  qWarning("in convertToPoint()");

  //get the current layer
  QgsMapLayer* theMapLayer = mIface->activeLayer();
  if(!theMapLayer)
    {
      QMessageBox::information(0, "no active layer", "this plugin needs an active point vector layer to make a conversion to points", QMessageBox::Ok);
      return;
    }
  QgsVectorLayer* theVectorLayer = dynamic_cast<QgsVectorLayer*>(theMapLayer);
  if(!theVectorLayer)
    {
      QMessageBox::information(0, "no vector layer", "this plugin needs an active point vector layer to make a conversion to points", QMessageBox::Ok);
      return;
    }

  QString fileName = QFileDialog::getSaveFileName();
  
  if(!fileName.isNull())
    {
      qWarning("the selected file name is: " + fileName);
      QFile f(fileName);
      if(!f.open(QIODevice::WriteOnly))
	{
	  QMessageBox::information(0, "error", "Could not open file", QMessageBox::Ok);
	}
      QTextStream theTextStream(&f);
      theTextStream.setRealNumberNotation(QTextStream::FixedNotation);
      //iterate over the layer features
      theTextStream << "x,y";
      //add the names of the non-geometry attributes
      std::vector<QgsField> fieldVector = theVectorLayer->getDataProvider()->fields();
      for(std::vector<QgsField>::const_iterator it = fieldVector.begin(); it != fieldVector.end(); ++it)
	{
	  theTextStream << "," << it->name();
	} 
      theTextStream << endl;
      
      QgsFeature* currentFeature = 0;
      geos::Geometry* currentGeometry = 0;
      geos::CoordinateSequence* currentSequence = 0;
      int currentSequenceSize = 0;

      //copy the attributes too
      QString featureAttributeString;
      std::vector<QgsFeatureAttribute> featureAttributeMap;

      theVectorLayer->getDataProvider()->reset();
      while(currentFeature = theVectorLayer->getNextFeature(true, false))
	{
	  featureAttributeMap = currentFeature->attributeMap();
	  featureAttributeString = "";
	  for(std::vector<QgsFeatureAttribute>::const_iterator it = featureAttributeMap.begin(); it != featureAttributeMap.end(); ++it)
	    {
	      featureAttributeString.append(","+it->fieldValue());
	      qWarning("appending " + it->fieldValue() + " to attribute map");
	    }
	  currentGeometry = currentFeature->geosGeometry();
	  currentSequence = currentGeometry->getCoordinates();
	  currentSequenceSize = currentSequence->getSize();
	  for(int i = 0; i < currentSequenceSize; ++i)
	    {
	      //qWarning("found coordinate: " + QString::number(currentSequence->getAt(i).x) + " // " + QString::number(currentSequence->getAt(i).y));
	      theTextStream << currentSequence->getAt(i).x << "," << currentSequence->getAt(i).y << featureAttributeString << endl;
	    }
	  delete currentGeometry;
	  delete currentFeature;
	}
    }
}

QGISEXTERN QgisPlugin* classFactory(QgisApp* app, QgisIface* iface)
{
  return new QgsPointConverterPlugin(app, iface);
}

QGISEXTERN QString name()
{
  return "point converter plugin";
}

QGISEXTERN QString description()
{
  return "A plugin that converts vector layers to delimited text point files";
}

QGISEXTERN QString version()
{
  return "0.00001";
}

// Return the type (either UI or MapLayer plugin)
QGISEXTERN int type()
{
  return QgisPlugin::UI;
}

// Delete ourself
QGISEXTERN void unload(QgisPlugin* theQgsPointConverterPluginPointer)
{
  delete theQgsPointConverterPluginPointer;
}

