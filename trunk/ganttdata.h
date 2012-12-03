/***************************************************************************
 *   Copyright (C) 2012 by Richard Crook                                   *
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

#ifndef GANTTDATA_H
#define GANTTDATA_H

#include <QDateTime>
#include <QVector>

class QPainter;

/*************************************************************************************************/
/************************** Data to support the drawing of a gantt task **************************/
/*************************************************************************************************/

class GanttData
{
public:
  GanttData();                                     // constructor

  void  setMilestone( QDateTime );                 // set task data to milestone
  void  setTask( QDateTime, QDateTime );           // set task data to simple gantt bar
  void  setSummary( QDateTime, QDateTime );        // set task data to summary gantt bar

  int   height( QPainter* );                                  // max height of task on gantt
  void  drawTask( QPainter*, int, QDateTime, double );        // draw task data on gantt
  void  drawTaskBar( QPainter*, int, QDateTime, double );     // draw task bar on gantt
  void  drawMilestone( QPainter*, int, QDateTime, double );   // draw milestone on gantt
  void  drawSummary( QPainter*, int, QDateTime, double );     // draw milestone on gantt

private:
  QDateTime            m_start;    // start of gantt task
  QVector<QDateTime>   m_end;      // span end
  QVector<float>       m_value;    // span value (-ve for summaries, +ve for tasks)
};

#endif // GANTTDATA_H
