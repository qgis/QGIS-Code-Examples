#ifndef QGSPOINTCONVERTERPLUGIN
#define QGSPOINTCONVERTERPLUGIN

#include "qgisplugin.h"
#include <QObject>

/**A plugin that converts vector layers to delimited text point files.
 The vertices of polygon/line type layers are converted to point features*/
class QgsPointConverterPlugin: public QObject, public QgisPlugin
{
 Q_OBJECT
 public:
  QgsPointConverterPlugin(QgisApp* app, QgisIface* iface);
  ~QgsPointConverterPlugin();
  /**Inserts an item into the toolbar*/
  virtual void initGui();
  void unload();

 private:
  QgisApp* mApp;
  QgisIface* mIface;
  QAction* mAction;

 private slots:
    void convertToPoint();
};

#endif
