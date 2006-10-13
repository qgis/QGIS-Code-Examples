#include "qgspointconverterplugin.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#ifdef WIN32
#define QGISEXTERN extern "C" __declspec( dllexport )
#else
#define QGISEXTERN extern "C"
#endif

QgsPointConverterPlugin::QgsPointConverterPlugin(QgisApp* app, QgisIface* iface): mIface(iface), mAction(0)
{

}

QgsPointConverterPlugin::~QgsPointConverterPlugin()
{

}

void QgsPointConverterPlugin::initGui()
{
  mAction = new QAction(tr("&Convert to point"), this);
  connect(mAction, SIGNAL(activated()), this, SLOT(convertToPoint()));
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
  qWarning("in method convertToPoint");
  QgsMapLayer* theMapLayer = mIface->activeLayer();
  if(!theMapLayer)
    {
      QMessageBox::information(0, tr("no active layer"), tr("this plugin needs an active point vector layer to make conversions to points"), QMessageBox::Ok);
      return;
    }
  QgsVectorLayer* theVectorLayer = dynamic_cast<QgsVectorLayer*>(theMapLayer);
  if(!theVectorLayer)
    {
      QMessageBox::information(0, tr("no vector layer"), tr("this plugin needs an active point vector layer to make conversions to points"), QMessageBox::Ok);
      return;
    }
  
  QString fileName = QFileDialog::getSaveFileName();
  if(!fileName.isNull())
    {
      qWarning("The selected filename is: " + fileName);
      QFile f(fileName);
      if(!f.open(QIODevice::WriteOnly))
      {
	QMessageBox::information(0, "error", "Could not open file", QMessageBox::Ok);
	return;
      }
      QTextStream theTextStream(&f);
      theTextStream.setRealNumberNotation(QTextStream::FixedNotation);
      
      theTextStream << "x,y" << endl;

      QgsFeature* currentFeature = 0;
      geos::Geometry* currentGeometry = 0;
      geos::CoordinateSequence* currentSequence = 0;
      int currentSequenceSize = 0;

      theVectorLayer->getDataProvider()->reset();
      //first parameter: fetch attributes, second parameter: selected features only
      while(currentFeature = theVectorLayer->getNextFeature(true, false))
	{
	  currentGeometry = currentFeature->geosGeometry();
	  currentSequence = currentGeometry->getCoordinates();
	  currentSequenceSize = currentSequence->getSize();
	  for(int i = 0; i < currentSequenceSize; ++i)
	    {
	      theTextStream << currentSequence->getAt(i).x << "," << currentSequence->getAt(i).y << endl;
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

