/***************************************************************************
 *   Copyright (C) 2013 by Richard Crook                                   *
 *   http://code.google.com/p/qplanner                                     *
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

#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QTabWidget>
#include <QModelIndexList>

class QItemSelectionModel;
class QXmlStreamWriter;

/*************************************************************************************************/
/***************************** Tabbed widget containing main screens *****************************/
/*************************************************************************************************/

namespace Ui { class MainTabWidget; }

class MainTabWidget : public QTabWidget
{
  Q_OBJECT
public:
  explicit MainTabWidget( QWidget* parent = nullptr );          // constructor
  ~MainTabWidget();                                             // destructor

  void  removePlanTab();             // remove 'Plan' tab
  void  updatePlan();                // update plan from 'Plan' tab widgets
  void  setModels();                 // ensure views are connected to correct models
  void  endEdits();                  // end any task/resource/calendar/day edits in progress
  void  updateGantt();               // trigger gantt widget redraw
  int   indexOfTasksTab();           // return index of tasks tab

  void  saveToStream( QXmlStreamWriter* );        // write display data to xml stream
  void  getGanttAttributes( QDateTime&,
                            QDateTime&,
                            double& );            // get gantt attributes start/end/secsPP
  void  setGanttAttributes( QDateTime,
                            QDateTime,
                            double );             // set gantt attributes start/end/secsPP

  QItemSelectionModel*  tasksSelectionModel();    // return selection model for tasks table view
  QModelIndexList       tasksSelectionIndexes();  // return selected indexes on tasks table view

public slots:
  void  slotUpdatePlanTab();         // ensure 'Plan' tab widgets are up-to-date

  void  slotEditTaskCell( const QModelIndex&,
                          const QString& );       // slot for editing task cell

private:
  Ui::MainTabWidget*   ui;           // user interface created using qt designer
};

#endif // MAINTABWIDGET_H
