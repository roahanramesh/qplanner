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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plan.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "calendarsmodel.h"
#include "daysmodel.h"
#include "task.h"
#include "tasksdelegate.h"

#include "commandpropertieschange.h"

#include <QUndoView>

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
  // initialise private variables
  m_undoview = NULL;

  // setup palette & ui for main window
  QPalette  pal = palette();
  pal.setBrush( QPalette::Inactive, QPalette::Highlight, QColor("#E0E0E0") );
  setPalette( pal );
  ui->setupUi( this );
  resize( 900, 450 );

  // set models & delegates for table views
  ui->tasksView->setModel( (QAbstractItemModel*)plan->tasks() );
  ui->tasksView->setItemDelegate( new TasksDelegate() );
  ui->resourcesView->setModel( (QAbstractItemModel*)plan->resources() );
  ui->calendarsView->setModel( (QAbstractItemModel*)plan->calendars() );
  ui->daysView->setModel( (QAbstractItemModel*)plan->days() );

  // set smaller row height for table views
  int height = ui->tasksView->fontMetrics().lineSpacing() + 3;
  ui->tasksView->verticalHeader()->setDefaultSectionSize( height );
  ui->resourcesView->verticalHeader()->setDefaultSectionSize( height );
  ui->calendarsView->verticalHeader()->setDefaultSectionSize( height );
  ui->daysView->verticalHeader()->setDefaultSectionSize( height );

  // set initial column widths for tables views
  ui->tasksView->horizontalHeader()->setDefaultSectionSize( 150 );
  ui->daysView->horizontalHeader()->setDefaultSectionSize( 70 );
  ui->calendarsView->horizontalHeader()->setDefaultSectionSize( 150 );
  plan->tasks()->setColumnWidths( ui->tasksView );
  plan->resources()->setColumnWidths( ui->resourcesView );
  plan->days()->setColumnWidths( ui->daysView );

  // setup tasks gantt
  ui->ganttView->createGantt( ui->ganttWidget );
  ui->ganttView->setTable( ui->tasksView );

  // create new palette for read-only edit widgets with different Base colour
  QPalette*     palette = new QPalette( ui->propertiesWidget->palette() );
  palette->setColor( QPalette::Base, palette->window().color() );

  // setup properties tab
  ui->planEnd->setReadOnly( true );
  ui->planEnd->setPalette( const_cast<const QPalette&>(*palette) );
  ui->fileName->setReadOnly( true );
  ui->fileName->setPalette( const_cast<const QPalette&>(*palette) );
  ui->fileLocation->setReadOnly( true );
  ui->fileLocation->setPalette( const_cast<const QPalette&>(*palette) );
  ui->savedBy->setReadOnly( true );
  ui->savedBy->setPalette( const_cast<const QPalette&>(*palette) );
  ui->savedWhen->setReadOnly( true );
  ui->savedWhen->setPalette( const_cast<const QPalette&>(*palette) );

  // ensure properties widget and plan variables are kept up-to-date
  slotUpdatePropertiesWidgets();
  connect( ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)) );
  connect( plan, SIGNAL(signalPropertiesUpdated()), this, SLOT(slotUpdatePropertiesWidgets()) );

  // construct Edit menu with undostack undo & redo actions at the top
  QAction* undoAction = plan->undostack()->createUndoAction( this );
  undoAction->setShortcut( QKeySequence::Undo );
  undoAction->setStatusTip( "Undo the last operation" );
  ui->menuEdit->addAction( undoAction );
  QAction* redoAction = plan->undostack()->createRedoAction( this );
  redoAction->setShortcut( QKeySequence::Redo );
  redoAction->setStatusTip( "Redo the last operation" );
  ui->menuEdit->addAction( redoAction );
  ui->menuEdit->addSeparator();
  ui->menuEdit->addAction( ui->actionInsert );
  ui->menuEdit->addSeparator();
  ui->menuEdit->addAction( ui->actionCut );
  ui->menuEdit->addAction( ui->actionCopy );
  ui->menuEdit->addAction( ui->actionPaste );
  ui->menuEdit->addAction( ui->actionDelete );
  ui->menuEdit->addSeparator();
  ui->menuEdit->addAction( ui->actionFindReplace );
  delete ui->menuEditTemp;

  // connect signal for tasks selection & data change to slots
  connect( ui->tasksView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT(slotTaskSelectionChanged(QItemSelection,QItemSelection)) );
  connect ( plan->tasks(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotTaskDataChanged(QModelIndex,QModelIndex)) );
}

/************************************** slotTaskDataChanged **************************************/

void MainWindow::slotTaskDataChanged( const QModelIndex& index, const QModelIndex& )
{
  // check if data change affects indent/outdent
  if ( index.column() == Task::SECTION_TITLE )
    slotTaskSelectionChanged( QItemSelection(), QItemSelection() );
}

/*********************************** slotTaskSelectionChanged ************************************/

void MainWindow::slotTaskSelectionChanged( const QItemSelection&, const QItemSelection& )
{
  // task selection has changed, determine unique rows selected
  QSet<int>  rows;
  foreach( QModelIndex index, ui->tasksView->selectionModel()->selection().indexes() )
    rows.insert( index.row() );

  // ensure indent & outdent actions only enabled is action possible
  bool inEnable  = false;
  bool outEnable = false;

  foreach( int row, rows )
  {
    if ( plan->tasks()->canIndent( row ) )  inEnable = true;
    if ( plan->tasks()->canOutdent( row ) ) outEnable = true;
  }

  ui->actionIndent->setEnabled( inEnable );
  ui->actionOutdent->setEnabled( outEnable );
}

/******************************************* slotIndent ******************************************/

void MainWindow::slotIndent()
{
  // indent task(s) - determine unique rows selected
  QSet<int>  rows;
  foreach( QModelIndex index, ui->tasksView->selectionModel()->selection().indexes() )
    rows.insert( index.row() );

  plan->tasks()->indentRows( rows );
}

/****************************************** slotOutdent ******************************************/

void MainWindow::slotOutdent()
{
  // outdent task(s) - determine unique rows selected
  QSet<int>  rows;
  foreach( QModelIndex index, ui->tasksView->selectionModel()->selection().indexes() )
    rows.insert( index.row() );

  plan->tasks()->outdentRows( rows );
}

/**************************************** slotSchedulePlan ***************************************/

void MainWindow::slotSchedulePlan()
{
  // get plan to reschedule all the tasks
  plan->tasks()->schedule();
}

/*************************************** slotViewUndoStack ***************************************/

void MainWindow::slotUndoStackView( bool checked )
{
  // show undo stack view window if checked, otherwise hide
  if ( checked )
  {
    if ( m_undoview == NULL )
    {
      m_undoview = new QUndoView( plan->undostack() );
      m_undoview->setWindowTitle( "Undo stack" );
      m_undoview->setAttribute( Qt::WA_QuitOnClose, false );
      m_undoview->setAttribute( Qt::WA_DeleteOnClose, true );
      connect( m_undoview, SIGNAL(destroyed()), this, SLOT(slotUndoStackViewDestroyed()) );
    }
    m_undoview->show();
  }
  else
  {
    if ( m_undoview ) m_undoview->hide();
  }
}

/*********************************** slotUndoStackViewDestroyed **********************************/

void MainWindow::slotUndoStackViewDestroyed()
{
  // undo stack view window closed and destroyed so reset pointer and uncheck action
  m_undoview = 0;
  ui->actionUndoStackView->setChecked( false );
}

/***************************************** slotTabChange *****************************************/

void MainWindow::slotTabChange( int index )
{
  // check if we need to update plan from 'Properties' tab widgets
  if ( ui->title->text()              != plan->title()          ||
       ui->planStart->dateTime()      != plan->start()          ||
       ui->defaultCal->currentIndex() != plan->index( plan->calendar() ) ||
       ui->dateTimeFormat->text()     != plan->datetimeFormat() ||
       ui->notesEdit->toPlainText()   != plan->notes() )
  {
    plan->undostack()->push( new CommandPropertiesChange(
                               ui->title->text(),              plan->title(),
                               ui->planStart->dateTime(),      plan->start(),
                               ui->defaultCal->currentIndex(), plan->index( plan->calendar() ),
                               ui->dateTimeFormat->text(),     plan->datetimeFormat(),
                               ui->notesEdit->toPlainText(),   plan->notes()) );


    return;
  }

  // otherwise just update the 'Properties' tab widgets
  slotUpdatePropertiesWidgets();
}

/************************************ updatePropertiesWidgets ************************************/

void MainWindow::slotUpdatePropertiesWidgets()
{
  // ensure 'Properties' tab widgets are up-to-date with what is in plan
  ui->title->setText( plan->title() );
  ui->title->setCursorPosition( 0 );

  ui->planStart->setDateTime( plan->start() );
  ui->planStart->setToolTip( plan->start().toString( plan->datetimeFormat() ) );

  ui->planEnd->setText( plan->end().toString("dd/MM/yyyy hh:mm:ss") );
  ui->planEnd->setCursorPosition( 0 );
  ui->planEnd->setToolTip( plan->end().toString( plan->datetimeFormat() ) );

  ui->defaultCal->clear();
  ui->defaultCal->addItems( plan->calendars()->namesList() );
  ui->defaultCal->setCurrentIndex( plan->index( plan->calendar() ) );

  ui->dateTimeFormat->setText( plan->datetimeFormat() );
  ui->dateTimeFormat->setCursorPosition( 0 );
  ui->dateTimeFormat->setToolTip( QDateTime::currentDateTime().toString( plan->datetimeFormat() ));

  ui->fileName->setText( plan->filename() );
  ui->fileName->setCursorPosition( 0 );

  ui->fileLocation->setText( plan->fileLocation() );
  ui->fileLocation->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->fileLocation() );

  ui->savedBy->setText( plan->savedBy() );
  ui->savedBy->setCursorPosition( 0 );

  ui->savedWhen->setText( plan->savedWhen().toString("dd/MM/yyyy hh:mm:ss") );
  ui->savedWhen->setCursorPosition( 0 );
  ui->savedWhen->setToolTip( plan->savedWhen().toString( plan->datetimeFormat() ) );

  ui->numTasks->setText( QString(": %1").arg( plan->numTasks() ) );
  ui->numResources->setText( QString(": %1").arg( plan->numResources() ) );
  ui->numCalendars->setText( QString(": %1").arg( plan->numCalendars() ) );
  ui->numDays->setText( QString(": %1").arg( plan->numDays() ) );

  ui->notesEdit->setPlainText( plan->notes() );
}
