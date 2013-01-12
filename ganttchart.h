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

#ifndef GANTTCHART_H
#define GANTTCHART_H

class Calendar;
class QTableView;
class PlanModel;

#include <QWidget>
#include <QDateTime>

/*************************************************************************************************/
/*********************** GanttChart provides a view of the plan gantt chart **********************/
/*************************************************************************************************/

class GanttChart : public QWidget
{
  Q_OBJECT
public:
  GanttChart( QWidget* );                          // constructor
  int  chartWidth();                               // return width in pixels of the gantt
  void setStart( QDateTime );                      // set start date-time for the gantt
  void setEnd( QDateTime );                        // set end date-time for the gantt
  void setSecsPerPixel( double );                  // set seconds per pixel scale
  void setTable( QTableView* );                    // set associated table view & plan for the gantt

public slots:
  void tasksScrolled( int );                       // receive vertical scroll events from table
  void taskHeight( int, int, int );                // receive row height change events from table
  void taskMoved( int, int, int );                 // receive task row moved events from table
  void taskChanged( int );                         // receive task change events from model

protected:
  void paintEvent( QPaintEvent* );                 // draw gantt contents

private:
  QDateTime      m_start;                          // start date-time for GanttChart
  QDateTime      m_end;                            // start date-time for GanttChart
  double         m_secsPP;                         // seconds per pixel
  QTableView*    m_table;                          // table view associated with the gantt

  void shadeNonWorkingDays( QPainter*,
         int, int, int, int );                     // shade gantt chart non working days
  void drawTasks( QPainter*,
         int, int, int, int );                     // draw gantt tasks
};

#endif // GANTTCHART_H
