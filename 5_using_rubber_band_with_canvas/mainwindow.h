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
#ifndef OMGMAINWINDOW_H
#define OMGMAINWINDOW_H

//QGis Includes
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
//
// Needed for rubber band support
//
#include <qgsrubberband.h>

//QT Includes
#include <QtGui>

//Local Includes
#include <ui_mainwindowbase.h>

/**
@author Tim Sutton
*/
class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
  Q_OBJECT;
    public:
      MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
      ~MainWindow();
    public slots:
      void zoomInMode();
      void zoomOutMode();
      void panMode();
      void addLayer();
      // next are tools overloaded from base class
      void on_mpToolShowRubberBand_clicked();
      void on_mpToolHideRubberBand_clicked();
      
    private:
     QgsMapCanvas  * mpMapCanvas;
     QVBoxLayout   * mpLayout;
     QToolBar      * mpMapToolBar;
     QgsMapTool    * mpPanTool;
     QgsMapTool    * mpZoomInTool;
     QgsMapTool    * mpZoomOutTool;
     QgsRubberBand * mpRubberBand;
};

#endif
