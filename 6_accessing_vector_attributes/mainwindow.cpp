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
#include <qgsfeature.h>
#include <qgsfeatureattribute.h>
#include <qgsfield.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorlayer.h>


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

  addLayer();
}

MainWindow::~MainWindow()
{
}

void MainWindow::addLayer()
{
  QString myLayerPath         = "data";
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

  //get the field list associated with the layer
  std::vector<QgsField> myFields = mypLayer->fields();
  //we will hold the list of attributes in this string
  QString myString;
  //print out the field names first
  for (unsigned int i = 0; i < myFields.size(); i++ )
  {
    //a little logic so we can produce output like : 
    // "foo","bar","etc"
    if (i==0)
    {
      //                 here is where we actually get the field value
      myString = "\"" +  myFields[i].name().trimmed() + "\"";
    }
    else
    {
      myString += ",\"" +  myFields[i].name().trimmed() + "\"";
    }
  }
  textBrowser->append("Field List: " + myString);
  //get the provider associated with the layer
  //the provider handles data io and is a plugin in qgis.
  QgsVectorDataProvider* mypProvider=mypLayer->getDataProvider();
  //check the provider is valid
  if(mypProvider)
  {
    //create a holder for retrieving features from the provider
    QgsFeature *mypFeature;
    //get the provider ready to iterate through it
    mypProvider->reset();
    //now iterate through each feature
    while ((mypFeature = mypProvider->getNextFeature(true)))
    {
      //get the attributes of this feature
      const std::vector < QgsFeatureAttribute >& myAttributes = mypFeature->attributeMap();
      //now loop through the attributes
      for (int i = 0; i < myAttributes.size(); i++)
      {
        //a little logic so we can produce output like : 
        // "foo","bar","etc"
        if (i==0)
        {
          //                 here is where we actually get the field value
          myString = "\"" +  myAttributes[i].fieldValue().trimmed() + "\"";
        }
        else
        {
          myString += ",\"" +  myAttributes[i].fieldValue().trimmed() + "\"";
        }
      }
      textBrowser->append("Field Values: " + myString); 
      //clean up before moving on to the next feature
      delete mypFeature;
    }
  }
}

