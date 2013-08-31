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

#ifndef GANTTVIEW_H
#define GANTTVIEW_H

#include <QScrollArea>
#include <QDateTime>
#include <QPoint>

class GanttChart;
class GanttScale;
class QTableView;

/*************************************************************************************************/
/**************************** GanttView shows tasks in a gantt format ****************************/
/*************************************************************************************************/

// TODO maybe 'setTablePlan' should be renamed 'setTable' ?

class GanttView : public QScrollArea
{
  Q_OBJECT
public:
  GanttView( QWidget* );                // constructor

  void    createGantt( QWidget* );      // sets the table associated with the gantt
  void    setTable( QTableView* );      // sets the table associated with the gantt
  int     scaleHeight();                // return scale (upper+lower) height

public slots:
  void    contextMenu( QPoint );        // slot to receive context menu signals
  void    slotZoomIn();
  void    slotZoomOut();
  void    slotZoomFit();

protected:
  void resizeEvent( QResizeEvent* );    // resize the GanttView contents

private:
  QWidget*        m_view;               // widget to display the gantt scales + chart
  GanttScale*     m_upperScale;         // gantt upper scale view
  GanttScale*     m_lowerScale;         // gantt lower scale view
  GanttChart*     m_chart;              // gantt chart view
  double          m_secsPP;             // seconds per pixel
  QDateTime       m_start;              // start date-time
  QDateTime       m_end;                // end date-time
};

#endif // GANTTVIEW_H
