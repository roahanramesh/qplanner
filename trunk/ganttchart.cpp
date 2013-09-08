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

#include "ganttchart.h"
#include "calendar.h"
#include "tasksmodel.h"
#include "task.h"
#include "plan.h"

#include <QPaintEvent>
#include <QPainter>
#include <QTableView>

/*************************************************************************************************/
/*********************** GanttChart provides a view of the plan gantt chart **********************/
/*************************************************************************************************/

/******************************************* constuctor ******************************************/

GanttChart::GanttChart( QWidget* parent ) : QWidget( parent )
{
  // set chart to have static contents
  setAttribute( Qt::WA_StaticContents );
  setAttribute( Qt::WA_NoSystemBackground );

  // set private veriables default values
  m_secsPP   = 100.0;
  m_table    = nullptr;
}

/****************************************** chartWidth *******************************************/

int GanttChart::chartWidth()
{
  // return width in pixels of the gantt
  return int( m_start.secsTo( m_end ) / m_secsPP );
}

/******************************************* setStart ********************************************/

void GanttChart::setStart( QDateTime start )
{
  // set private variable m_start date-time for gantt
  m_start = start;
}

/******************************************** setEnd *********************************************/

void GanttChart::setEnd( QDateTime end )
{
  // set private variable m_end date-time for gantt
  m_end = end;
}

/**************************************** setSecsPerPixel ****************************************/

void GanttChart::setSecsPerPixel( double spp )
{
  // set private variable seconds per pixel scale, and update the display
  m_secsPP = spp;
  update();
}

/******************************************* setTable ********************************************/

void GanttChart::setTable( QTableView* table )
{
  // disconnect previous m_table signals
  if ( m_table )
  {
    // disconnect m_table vertical header to chart for task row height change
    disconnect( (QObject*)m_table->verticalHeader(), SIGNAL(sectionResized(int,int,int)),
                 this, SLOT(slotTaskHeightChanged(int,int,int)) );

    // disconnect m_table vertical header to chart for vertical moving of tasks
    disconnect( (QObject*)m_table->verticalHeader(), SIGNAL(sectionMoved(int,int,int)),
                 this, SLOT(slotTaskMoved(int,int,int)) );

    // disconnect m_table vertical scroll bar to chart for vertical scrolling of tasks
    disconnect( (QObject*)m_table->verticalScrollBar(), SIGNAL(valueChanged(int)),
                 this, SLOT(slotTasksScrolled(int)) );
  }

  // set private variables with table & plan associated with gantt
  m_table    = table;

  // connect signals for newly associated m_table
  if ( m_table )
  {
    // connect m_table vertical header to chart for task row height change
    connect( (QObject*)m_table->verticalHeader(), SIGNAL(sectionResized(int,int,int)),
              this, SLOT(slotTaskHeightChanged(int,int,int)),
              Qt::UniqueConnection );

    // connect m_table vertical header to chart for vertical moving of tasks
    connect( (QObject*)m_table->verticalHeader(), SIGNAL(sectionMoved(int,int,int)),
              this, SLOT(slotTaskMoved(int,int,int)),
              Qt::UniqueConnection );

    // connect m_table vertical scroll bar to chart for vertical scrolling of tasks
    connect( (QObject*)m_table->verticalScrollBar(), SIGNAL(valueChanged(int)),
              this, SLOT(slotTasksScrolled(int)),
              Qt::UniqueConnection );
  }

  // connect plan model gantt changes for general task updates
  connect( plan->tasks(), SIGNAL(ganttChanged()), this, SLOT(slotTasksChanged()),
           Qt::UniqueConnection );
}

/***************************************** tasksScrolled *****************************************/

void GanttChart::slotTasksScrolled( int value )
{
  Q_UNUSED(value);

  // update whole chart to reflect vertical scrolling of tasks m_table
  update();
}

/******************************************* taskMoved *******************************************/

void GanttChart::slotTaskMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex )
{
  Q_UNUSED(logicalIndex);
  Q_UNUSED(oldVisualIndex);
  Q_UNUSED(newVisualIndex);

  // update whole chart (could do less?) to reflect row moved on tasks m_table
  update();
}

/******************************************* taskHeight ******************************************/

void GanttChart::slotTaskHeightChanged( int row, int oldHeight, int newHeight )
{
  Q_UNUSED(oldHeight);
  Q_UNUSED(newHeight);

  // update chart at and below row when task m_table row height change
  update( 0, m_table->rowViewportPosition(row) - 4, width(), height() );
}

/****************************************** taskInserted *****************************************/

void GanttChart::slotTasksChanged()
{
  // update whole chart
  update();
}

/******************************************* paintEvent ******************************************/

void GanttChart::paintEvent( QPaintEvent* event )
{
  // get the clipping rectangle
  int x = event->rect().x();
  int y = event->rect().y();
  int w = event->rect().width();
  int h = event->rect().height();

  QPainter p( this );
  shadeNonWorkingDays( &p, x, y, w, h );
  drawTasks( &p, y, h );
  drawDependencies( &p, y, h );

  // draw current date-time line
  int now = int(m_start.secsTo( QDateTime::currentDateTime() ) / m_secsPP);
  p.setPen( Qt::DotLine );
  p.drawLine( now, y, now, y+h );
}

/**************************************** drawDependencies ***************************************/

void GanttChart::drawDependencies( QPainter* p, int y, int h )
{
  // determine first and last task visible
  int first = m_table->rowAt( y );
  int last  = m_table->rowAt( y+h );
  if ( first < 0 ) first = 0;
  if ( last  < 0 ) last  = plan->tasks()->rowCount() - 1;

  // for each task
  for( int t=0 ; t<=plan->tasks()->rowCount()-1 ; t++ )
  {
    Task*        task  = plan->task(t);
    int          thisY = m_table->rowViewportPosition(t) + ( m_table->rowHeight(t) / 2 );
    QStringList  preds = task->predecessors().split(",", QString::SkipEmptyParts);

    // and each task predecessor
    foreach( QString pred, preds )
    {
      // extract task number at beginning of predecessor string and type
      QString type = Predecessors::LABEL_FINISH_START;   // default predecessor type
      int     num = 0;
      for( int c=0 ; c<pred.length() ; c++ )
      {
        if ( pred.at(c).isDigit() )
          num = num*10 + pred.at(c).digitValue();
        else
        {
          // not a digit, so must be a type
          type = QString( pred.at(c) ) + QString( pred.at(c+1) );
          break;
        }
      }

      // if link outside range to be drawn, move on to next
      if ( t < first && num < first ) continue;
      if ( t > last  && num > last  ) continue;

      int otherY = m_table->rowViewportPosition(num) + ( m_table->rowHeight(num) / 2 );

      if ( type == Predecessors::LABEL_FINISH_START )
        task->ganttData()->drawDependencyFS( p, thisY, otherY, num, m_start, m_secsPP );

      if ( type == Predecessors::LABEL_START_FINISH )
        task->ganttData()->drawDependencySF( p, thisY, otherY, num, m_start, m_secsPP );

      if ( type == Predecessors::LABEL_FINISH_FINISH )
        task->ganttData()->drawDependencyFF( p, thisY, otherY, num, m_start, m_secsPP );

      if ( type == Predecessors::LABEL_START_START )
        task->ganttData()->drawDependencySS( p, thisY, otherY, num, m_start, m_secsPP );
    }
  }
}

/******************************************* drawTasks *******************************************/

void GanttChart::drawTasks( QPainter* p, int y, int h )
{
  // determine first and last task to draw
  int first = m_table->rowAt( y );
  int last  = m_table->rowAt( y+h );
  if ( first < 0 ) first = 0;
  if ( last  < 0 ) last  = plan->tasks()->rowCount() - 1;

  // for each row draw the gantt task
  for( int row=first ; row<=last ; row++ )
    plan->task(row)->ganttData()->drawTask( p, m_table->rowViewportPosition(row) + ( m_table->rowHeight(row) / 2 ), m_start, m_secsPP );
}

/*************************************** shadeNonWorkingDays **************************************/

void GanttChart::shadeNonWorkingDays( QPainter* p, int x, int y, int w, int h )
{
  // fill in white background
  p->fillRect( x, y, w, h, Qt::white );

  // not practical to shade non working days if one day less than one pixel
  if ( m_secsPP > 86400 ) return;

  // use plan current default calendar
  Calendar*  calendar = plan->calendar();

  // setup internal variable
  QDate  dateStart = m_start.addSecs( int( m_secsPP * (x-1) ) ).date();
  QDate  dateEnd   = m_start.addSecs( int( m_secsPP * (x+w) ) ).date();
  int    xs = -1, xe;
  QBrush brush( QColor("#F5F5F5") );

  // for each date check if working and shade accordingly
  for( QDate date = dateStart; date <= dateEnd; date=date.addDays(1) )
  {
    // find m_start of non-working days
    if ( xs<0 && !calendar->isWorking(date) )
    {
      xs = int( m_start.secsTo( QDateTime(date) ) / m_secsPP ) + 1;
      if ( xs < 0 ) xs = 0;
    }

    // find m_end of non-working days and shade the period
    if ( xs>=0 && calendar->isWorking(date) )
    {
      xe = int( m_start.secsTo( QDateTime(date) ) / m_secsPP );
      p->fillRect( xs, y, xe-xs, h, brush );
      xs = -1;
    }
  }

  // shade any remaining non-working days
  if (xs>=0) p->fillRect( xs, y, width(), h, brush );
}
