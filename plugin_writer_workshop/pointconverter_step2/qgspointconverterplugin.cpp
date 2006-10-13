#include "qgspointconverterplugin.h"

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

