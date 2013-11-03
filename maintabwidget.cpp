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

#include "maintabwidget.h"
#include "ui_maintabwidget.h"
#include "xdatetime.h"

#include "plan.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "calendarsmodel.h"
#include "daysmodel.h"

#include "tasksdelegate.h"
#include "resourcesdelegate.h"

#include "commandpropertieschange.h"

#include <QMessageBox>
#include <QXmlStreamWriter>

/*************************************************************************************************/
/***************************** Tabbed widget containing main screens *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainTabWidget::MainTabWidget( QWidget* parent ) : QTabWidget( parent ), ui( new Ui::MainTabWidget )
{
  // setup palette & ui for main tab widget
  QPalette  pal = palette();
  pal.setBrush( QPalette::Inactive, QPalette::Highlight, QColor("#E0E0E0") );
  setPalette( pal );
  ui->setupUi( this );

  // set models & delegates for table views
  setModels();
  TasksDelegate*      td = new TasksDelegate();
  ResourcesDelegate*  rd = new ResourcesDelegate();
  ui->tasksView->setItemDelegate( td );
  ui->resourcesView->setItemDelegate( rd );

  // connect task delegate edit task cell to slot, queued so any earlier edit is finished and closed
  connect( td, SIGNAL(editTaskCell(QModelIndex,QString)),
           this, SLOT(slotEditTaskCell(QModelIndex,QString)), Qt::QueuedConnection );

  // hide task 0 'plan summary' and resource 0 'unassigned'
  ui->tasksView->verticalHeader()->hideSection( 0 );
  ui->resourcesView->verticalHeader()->hideSection( 0 );

  // set initial column widths for tables views
  plan->tasks()->setColumnWidths( ui->tasksView );
  plan->resources()->setColumnWidths( ui->resourcesView );
  ui->calendarsView->horizontalHeader()->setDefaultSectionSize( 150 );
  plan->days()->setColumnWidths( ui->daysView );

  // set tasks view splitter behaviour & default position
  ui->tasksGanttSplitter->setStretchFactor( 1, 1 );
  QList<int> sizes = ui->tasksGanttSplitter->sizes();
  sizes[0] = ui->tasksView->horizontalHeader()->sectionSize( 0 ) +
             ui->tasksView->horizontalHeader()->sectionSize( 1 ) +
             ui->tasksView->horizontalHeader()->sectionSize( 2 ) +
             ui->tasksView->verticalHeader()->width();
  sizes[1] = sizes[0];
  ui->tasksGanttSplitter->setSizes( sizes );

  // setup tasks gantt
  ui->ganttView->createGantt( ui->ganttWidget );
  ui->ganttView->setTable( ui->tasksView );
  ui->tasksView->setHeaderHeight( ui->ganttView->scaleHeight() );

  // create new palette for read-only edit widgets with different Base colour
  QPalette*     palette = new QPalette( ui->propertiesWidget->palette() );
  palette->setColor( QPalette::Base, palette->window().color() );

  // setup plan tab
  ui->planBeginning->setPalette( *palette );
  ui->planEnd->setPalette( *palette );
  ui->fileName->setPalette( *palette );
  ui->fileLocation->setPalette( *palette );
  ui->savedBy->setPalette( *palette );
  ui->savedWhen->setPalette( *palette );
  slotUpdatePlanTab();
}

/****************************************** destructor *******************************************/

MainTabWidget::~MainTabWidget()
{
  // free up memory used by the ui
  delete ui;
}

/**************************************** removePlanTab ******************************************/

void MainTabWidget::removePlanTab()
{
  // remove 'Plan' tab (for example of new windows)
  removeTab( this->indexOf( ui->planTab ) );
}

/**************************************** updatePlanTab ******************************************/

void MainTabWidget::slotUpdatePlanTab()
{
  // ensure 'Plan' tab widgets are up-to-date with what is in plan
  ui->title->setText( plan->title() );
  ui->title->setCursorPosition( 0 );

  ui->planBeginning->setText( plan->beginning().toString("dd/MM/yyyy hh:mm:ss") );
  ui->planBeginning->setCursorPosition( 0 );
  ui->planBeginning->setToolTip( plan->beginning().toString( plan->datetimeFormat() ) );

  ui->planStart->setDateTime( plan->start() );
  ui->planStart->setToolTip( plan->start().toString( plan->datetimeFormat() ) );

  ui->planEnd->setText( plan->end().toString("dd/MM/yyyy hh:mm:ss") );
  ui->planEnd->setCursorPosition( 0 );
  ui->planEnd->setToolTip( plan->end().toString( plan->datetimeFormat() ) );

  ui->defaultCalendar->clear();
  ui->defaultCalendar->addItems( plan->calendars()->namesList() );
  ui->defaultCalendar->setCurrentIndex( plan->index( plan->calendar() ) );

  ui->dateTimeFormat->setText( plan->datetimeFormat() );
  ui->dateTimeFormat->setCursorPosition( 0 );
  ui->dateTimeFormat->setToolTip( QDateTime::currentDateTime().toString( plan->datetimeFormat() ));

  ui->fileName->setText( plan->filename() );
  ui->fileName->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->filename() );

  ui->fileLocation->setText( plan->fileLocation() );
  ui->fileLocation->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->fileLocation() );

  ui->savedBy->setText( plan->savedBy() );
  ui->savedBy->setCursorPosition( 0 );
  ui->savedBy->setToolTip( plan->savedBy() );

  ui->savedWhen->setText( plan->savedWhen().toString("dd/MM/yyyy hh:mm:ss") );
  ui->savedWhen->setCursorPosition( 0 );
  ui->savedWhen->setToolTip( plan->savedWhen().toString( plan->datetimeFormat() ) );

  ui->numTasks->setText( QString(": %1").arg( plan->numTasks() ) );
  ui->numResources->setText( QString(": %1").arg( plan->numResources() ) );
  ui->numCalendars->setText( QString(": %1").arg( plan->numCalendars() ) );
  ui->numDays->setText( QString(": %1").arg( plan->numDays() ) );

  ui->notesEdit->setPlainText( plan->notes() );
}

/*************************************** slotEditTaskCell ****************************************/

void MainTabWidget::slotEditTaskCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable task cell edit to be automatically re-started after validation failure
  setCurrentWidget( ui->tasksGanttTab );
  ui->tasksView->setCurrentIndex( index );
  QMessageBox::warning( ui->tasksView, "QPlanner", warning, QMessageBox::Retry );
  ui->tasksView->edit( index );

  // clear override
  plan->tasks()->setOverride( QModelIndex(), QString() );
}

/***************************************** updatePlan ********************************************/

void MainTabWidget::updatePlan()
{
  // check if we need to update plan from 'Plan' tab widgets
  if ( ui->title->text()                   != plan->title()          ||
       ui->planStart->dateTime()           != plan->start()          ||
       ui->defaultCalendar->currentIndex() != plan->index( plan->calendar() ) ||
       ui->dateTimeFormat->text()          != plan->datetimeFormat() ||
       ui->notesEdit->toPlainText()        != plan->notes() )
  {
    plan->undostack()->push( new CommandPropertiesChange(
                               ui->title->text(),                   plan->title(),
                               ui->planStart->dateTime(),           plan->start(),
                               ui->defaultCalendar->currentIndex(), plan->index( plan->calendar() ),
                               ui->dateTimeFormat->text(),          plan->datetimeFormat(),
                               ui->notesEdit->toPlainText(),        plan->notes()) );
  }
}

/****************************************** setModels ********************************************/

void MainTabWidget::setModels()
{
  // ensure table views are connected to correct models
  ui->tasksView->setModel( (QAbstractItemModel*)plan->tasks() );
  ui->resourcesView->setModel( (QAbstractItemModel*)plan->resources() );
  ui->calendarsView->setModel( (QAbstractItemModel*)plan->calendars() );
  ui->daysView->setModel( (QAbstractItemModel*)plan->days() );
  ui->ganttView->setTable( ui->tasksView );
}

/****************************************** endEdits *********************************************/

void MainTabWidget::endEdits()
{
  // end any task/resource/calendar/day edits in progress
  ui->tasksView->endEdit();
  ui->resourcesView->endEdit();
  ui->calendarsView->endEdit();
  ui->daysView->endEdit();
}

/****************************************** updateGantt ******************************************/

void MainTabWidget::updateGantt()
{
  // trigger gantt widget redraw
  ui->ganttWidget->update();
}

/**************************************** indexOfTasksTab ****************************************/

int MainTabWidget::indexOfTasksTab()
{
  // return index of tasks tab
  return indexOf( ui->tasksGanttTab );
}

/***************************************** saveToStream ******************************************/

void MainTabWidget::saveToStream( QXmlStreamWriter* stream )
{
  // write display data to xml stream
  stream->writeStartElement( "display-data" );

  stream->writeStartElement( "gantt" );
  stream->writeAttribute( "start", XDateTime( ui->ganttView->start() ).toText() );
  stream->writeAttribute( "end", XDateTime( ui->ganttView->end() ).toText() );
  stream->writeAttribute( "secspp", QString("%1").arg( ui->ganttView->secsPP() ) );
  stream->writeAttribute( "upper-scale", "TODO" );
  stream->writeAttribute( "lower-scale", "TODO" );
  stream->writeEndElement();  // gantt

  stream->writeEndElement();  // display-data
}

/************************************** getGanttAttributes ***************************************/

void MainTabWidget::getGanttAttributes( QDateTime& start, QDateTime& end, double& secspp )
{
  // get gantt attributes start/end/secsPP
  start  = ui->ganttView->start();
  end    = ui->ganttView->end();
  secspp = ui->ganttView->secsPP();
}

/************************************** setGanttAttributes ***************************************/

void MainTabWidget::setGanttAttributes( QDateTime start, QDateTime end, double secspp )
{
  // set gantt attributes start/end/secsPP
  ui->ganttView->setStart( start );
  ui->ganttView->setEnd( end );
  ui->ganttView->setSecsPP( secspp );
  ui->ganttView->setWidth();
}

/************************************** tasksSelectionModel **************************************/

QItemSelectionModel* MainTabWidget::tasksSelectionModel()
{
  // return selection model for tasks table view
  return ui->tasksView->selectionModel();
}

/************************************* tasksSelectionIndexes *************************************/

QModelIndexList  MainTabWidget::tasksSelectionIndexes()
{
  // return selected indexes on tasks table view
  return ui->tasksView->selectionModel()->selection().indexes();
}
