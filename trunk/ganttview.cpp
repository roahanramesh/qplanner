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

#include "ganttview.h"
#include "ganttchart.h"
#include "ganttscale.h"
#include "xdatetime.h"
#include "plan.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QResizeEvent>

/*************************************************************************************************/
/**************************** GanttView shows tasks in a gantt format ****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

GanttView::GanttView( QWidget* parent ) : QScrollArea( parent )
{
  // set defaults
  m_secsPP = 10000.0;
  m_start  = QDateTime( QDate::currentDate().addDays(-7) );
  m_end    = QDateTime( QDate::currentDate().addDays(70) );
}

/******************************************* createGantt *****************************************/

void GanttView::createGantt( QWidget* view )
{
  // create widget to display the gantt scales + chart
  m_view = view;
  m_view->setContextMenuPolicy( Qt::CustomContextMenu );
  connect( m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)),
           Qt::UniqueConnection );

  // create the upper & lower scale view and chart view
  m_upperScale = new GanttScale( m_view );
  m_upperScale->setStart( m_start );
  m_upperScale->setInterval( XDateTime::INTERVAL_MONTH );
  m_upperScale->setLabelFormat( "MMMM-yyyy" );
  m_upperScale->setSecsPerPixel( m_secsPP );

  m_lowerScale = new GanttScale( m_view );
  m_lowerScale->setStart( m_start );
  m_lowerScale->setInterval( XDateTime::INTERVAL_WEEK );
  m_lowerScale->setLabelFormat( "dd-MMM" );
  m_lowerScale->setSecsPerPixel( m_secsPP );

  m_chart = new GanttChart( m_view );
  m_chart->setStart( m_start );
  m_chart->setEnd( m_end );
  m_chart->setSecsPerPixel( m_secsPP );

  // set layout to be vertical
  QVBoxLayout*  layout = new QVBoxLayout( m_view );
  layout->setMargin( 0 );
  layout->setSpacing( 0 );
  layout->addWidget( m_upperScale );
  layout->addWidget( m_lowerScale );
  layout->addWidget( m_chart );
  m_view->setLayout( layout );

  // set the scroll area to use the widget and look'n'feel
  setWidget( m_view );
  setFrameStyle( 0 );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

/******************************************* contextMenu *****************************************/

void GanttView::contextMenu( QPoint pos )
{
  // don't use pos as it gives position relative to widget not screen
  Q_UNUSED( pos );

  // create menu and associated actions for context menu
  QMenu menu;
  menu.setTitle( "Gantt" );
  menu.addAction( "Zoom in",  this, SLOT(slotZoomIn()) );
  menu.addAction( "Zoom out", this, SLOT(slotZoomOut()) );
  menu.addAction( "Zoom fit", this, SLOT(slotZoomFit()) );
  menu.addSeparator();
  m_upperScale->menu()->setTitle( "Upper scale" );
  menu.addMenu( m_upperScale->menu() );
  m_lowerScale->menu()->setTitle( "Lower scale" );
  menu.addMenu( m_lowerScale->menu() );
  menu.addAction( "Non working days" );
  menu.addAction( "Current date" );
  menu.addAction( "Upper scale mark" );
  menu.addAction( "Lower scale mark" );

  // execute the context menu
  menu.exec( QCursor::pos() );
}

/******************************************* slotZoomIn ******************************************/

void  GanttView::slotZoomIn()
{
  // decrease secs per pixel to zoom in on chart & scales
  m_secsPP = m_secsPP / 1.414;
  m_upperScale->setSecsPerPixel( m_secsPP );
  m_lowerScale->setSecsPerPixel( m_secsPP );
  m_chart->setSecsPerPixel( m_secsPP );

  // ensure view width is never less than chart width
  if ( m_chart->chartWidth() > width() ) m_view->setFixedWidth( m_chart->chartWidth() );
  else                                   m_view->setFixedWidth( width() );
}

/******************************************* slotZoomOut *****************************************/

void  GanttView::slotZoomOut()
{
  // increase secs per pixel to zoom out on chart & scales
  m_secsPP = m_secsPP * 1.414;
  m_upperScale->setSecsPerPixel( m_secsPP );
  m_lowerScale->setSecsPerPixel( m_secsPP );
  m_chart->setSecsPerPixel( m_secsPP );

  // zoom around centre of chart if chart width less than view width
  if ( m_chart->chartWidth() < width() )
  {
    QDateTime  start = m_start.addSecs( -m_secsPP * ( width() - m_chart->chartWidth() ) / 2 );
    m_upperScale->setStart( start );
    m_lowerScale->setStart( start );
    m_chart->setStart( start );
  }

  // ensure view width is never less than chart width
  if ( m_chart->chartWidth() > width() ) m_view->setFixedWidth( m_chart->chartWidth() );
  else                                   m_view->setFixedWidth( width() );
}

/******************************************* slotZoomFit *****************************************/

void  GanttView::slotZoomFit()
{
  // determine plan beginning and end to show, and set view new fixed width
  QDateTime  start  = plan->stretch( plan->beginning() );
  QDateTime  end    = plan->stretch( plan->end() );
  m_view->setFixedWidth( width() );

  // if start or end is invalid, eg before any tasks, keep existing start and secsPP
  if ( !start.isValid() || !end.isValid() )
  {
    m_end   = m_start.addSecs( width() * m_secsPP );
    m_chart->setEnd( m_end );
    return;
  }

  // add margins to start and end
  qint64 margin = 1 + start.secsTo( end ) / 32;
  m_start  = start.addSecs( -margin );
  m_end    = end.addSecs( margin );

  // set secs per pixel to show entire chart duration in displayed width
  m_secsPP = double( m_start.secsTo( m_end ) ) / width();

  // set chart
  m_chart->setStart( m_start );
  m_chart->setEnd( m_end );
  m_chart->setSecsPerPixel( m_secsPP );

  // set upper scale
  m_upperScale->setSecsPerPixel( m_secsPP );
  m_upperScale->setStart( m_start );

  // set lower scale
  m_lowerScale->setSecsPerPixel( m_secsPP );
  m_lowerScale->setStart( m_start );
}

/******************************************* resizeEvent *****************************************/

void GanttView::resizeEvent( QResizeEvent* event )
{
  // ensure view height just fits the scroll area height
  m_view->setFixedHeight( event->size().height() );

  // ensure view width is never less than chart width
  if ( m_chart->chartWidth() > width() ) m_view->setFixedWidth( m_chart->chartWidth() );
  else                                   m_view->setFixedWidth( width() );

  // call QScrollArea::resizeEvent to handle horizontal scroll bar etc
  QScrollArea::resizeEvent( event );
}

/****************************************** scaleHeight ******************************************/

int GanttView::scaleHeight()
{
  // return the height of the gantt scales (upper & lower)
  return m_upperScale->height() + m_lowerScale->height();
}

/******************************************* setTable ********************************************/

void GanttView::setTable( QTableView* table )
{
  // sets the table associated with the gantt
  m_chart->setTable( table );
}

/******************************************* setTable ********************************************/

void GanttView::updateChart( int row )
{
  // re-draw gantt chart (from row downwards)
  m_chart->taskChanged( row );
}
