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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plan.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "calendarsmodel.h"
#include "daysmodel.h"
#include "task.h"
#include "tasksdelegate.h"
#include "resourcesdelegate.h"

#include "commandpropertieschange.h"

#include <QUndoView>
#include <QMessageBox>
#include <QFileDialog>
#include <QXmlStreamWriter>

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
  // initialise private variables
  m_undoview = nullptr;

  // setup palette & ui for main window
  QPalette  pal = palette();
  pal.setBrush( QPalette::Inactive, QPalette::Highlight, QColor("#E0E0E0") );
  setPalette( pal );
  ui->setupUi( this );
  resize( 900, 450 );

  // set models & delegates for table views
  ui->tasksView->setModel( (QAbstractItemModel*)plan->tasks() );
  TasksDelegate*  td = new TasksDelegate();
  ui->tasksView->setItemDelegate( td );
  ui->resourcesView->setModel( (QAbstractItemModel*)plan->resources() );
  ResourcesDelegate*  rd = new ResourcesDelegate();
  ui->resourcesView->setItemDelegate( rd );
  ui->calendarsView->setModel( (QAbstractItemModel*)plan->calendars() );
  ui->daysView->setModel( (QAbstractItemModel*)plan->days() );

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

  // setup properties tab
  ui->planBeginning->setPalette( *palette );
  ui->planEnd->setPalette( *palette );
  ui->fileName->setPalette( *palette );
  ui->fileLocation->setPalette( *palette );
  ui->savedBy->setPalette( *palette );
  ui->savedWhen->setPalette( *palette );

  // update edit menu with undostack undo & redo actions
  setModels();
  slotTabChange( ui->mainTabWidget->currentIndex() );
}

/******************************************* setModels *******************************************/

void MainWindow::setModels()
{
  // ensure properties widget and plan variables are kept up-to-date
  slotUpdatePropertiesWidgets();
  connect( ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)),
           Qt::UniqueConnection );
  connect( plan, SIGNAL(signalPropertiesUpdated()), this, SLOT(slotUpdatePropertiesWidgets()),
           Qt::UniqueConnection );

  // set models for table views
  ui->tasksView->setModel( (QAbstractItemModel*)plan->tasks() );
  ui->resourcesView->setModel( (QAbstractItemModel*)plan->resources() );
  ui->calendarsView->setModel( (QAbstractItemModel*)plan->calendars() );
  ui->daysView->setModel( (QAbstractItemModel*)plan->days() );

  // set models for gantt view
  ui->ganttView->setTable( ui->tasksView );

  // set undostack for edit menu undo/redo
  QAction* undoAction = plan->undostack()->createUndoAction( this );
  undoAction->setShortcut( QKeySequence::Undo );
  undoAction->setStatusTip( "Undo the last operation" );
  ui->menuEdit->insertAction( ui->actionUndo, undoAction );
  QAction* redoAction = plan->undostack()->createRedoAction( this );
  redoAction->setShortcut( QKeySequence::Redo );
  redoAction->setStatusTip( "Redo the last operation" );
  ui->menuEdit->insertAction( ui->actionUndo, redoAction );
  ui->menuEdit->removeAction( ui->actionUndo );
  ui->menuEdit->removeAction( ui->actionRedo );
  ui->actionUndo = undoAction;
  ui->actionRedo = redoAction;

  // set undostack for undoview
  if ( m_undoview != nullptr ) m_undoview->setStack( plan->undostack() );

  // connect signal for tasks selection & data change to slots
  connect( ui->tasksView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT(slotTaskSelectionChanged(QItemSelection,QItemSelection)),
           Qt::UniqueConnection );
  connect( plan->tasks(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
           this, SLOT(slotTaskDataChanged(QModelIndex,QModelIndex)),
           Qt::UniqueConnection );
}

/******************************************* endEdits ********************************************/

void MainWindow::endEdits()
{
  // end any task/resource/calendar/day edits in progress
  ui->tasksView->endEdit();
  ui->resourcesView->endEdit();
  ui->calendarsView->endEdit();
  ui->daysView->endEdit();
}

/*************************************** slotEditTaskCell ****************************************/

void MainWindow::slotEditTaskCell( const QModelIndex& index, const QString& warning )
{
  // slot to enable task cell edit to be automatically re-started after validation failure
  ui->mainTabWidget->setCurrentWidget( ui->tasksGanttTab );
  ui->tasksView->setCurrentIndex( index );
  QMessageBox::warning( ui->tasksView, "QPlanner", warning );
  ui->tasksView->edit( index );

  // clear override
  plan->tasks()->setOverride( QModelIndex(), QString() );
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
  endEdits();
  QSet<int>  rows;
  foreach( QModelIndex index, ui->tasksView->selectionModel()->selection().indexes() )
    rows.insert( index.row() );

  plan->tasks()->indentRows( rows );
}

/****************************************** slotOutdent ******************************************/

void MainWindow::slotOutdent()
{
  // outdent task(s) - determine unique rows selected
  endEdits();
  QSet<int>  rows;
  foreach( QModelIndex index, ui->tasksView->selectionModel()->selection().indexes() )
    rows.insert( index.row() );

  plan->tasks()->outdentRows( rows );
}

/****************************************** slotFileNew ******************************************/

void MainWindow::slotFileNew()
{
  // slot for file new plan action
  endEdits();
  qDebug("MainWindow::slotFileNew() - TODO !!!!");
}

/****************************************** slotFileOpen *****************************************/

bool MainWindow::slotFileOpen()
{
  // slot for file open plan action - get user to select filename and location
  endEdits();
  QString filename = QFileDialog::getOpenFileName();
  if ( filename.isEmpty() ) return false;

  // open the file and check we can read from it
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    ui->statusBar->showMessage( QString("Failed to open '%1'").arg(filename) );
    return false;
  }

  // open an xml stream reader and try to load new plan data
  QXmlStreamReader  stream( &file );
  Plan*             newPlan = new Plan();
  Plan*             oldPlan = plan;
  plan = newPlan;   // set global plan variable so plan methods work as expected

  while ( !stream.atEnd() && !stream.isStartElement() )
    stream.readNext();

  if ( stream.isStartElement() )
  {
    if ( stream.name() == "qplanner" )
      newPlan->loadFromStream( &stream, filename );
    else
      stream.raiseError( QString("Unrecognised element '%1'").arg(stream.name().toString()) );
  }

  // close file, check if error occured while loading
  file.close();
  if ( stream.hasError() )
  {
    ui->statusBar->showMessage( QString("Failed to load '%1' (%2)").arg(filename).arg(stream.errorString()) );
    delete newPlan;
    plan = oldPlan;
    return false;
  }

  // check if plan is ok
  if ( !newPlan->isOK() )
  {
    ui->statusBar->showMessage( QString("Invalid plan in '%1'").arg(filename) );
    delete newPlan;
    plan = oldPlan;
    return false;
  }

  // delete old plan, set models, and schedule
  delete oldPlan;
  setModels();
  plan->tasks()->schedule();
  ui->statusBar->showMessage( QString("Loaded '%1'").arg(filename) );
  return true;
}

/****************************************** slotFileSave *****************************************/

void MainWindow::slotFileSave()
{
  // slot for file save plan action
  endEdits();
  qDebug("MainWindow::slotFileSave() - TODO !!!!");
}

/***************************************** slotFileSaveAs ****************************************/

bool MainWindow::slotFileSaveAs()
{
  // slot for file saveAs plan action - get user to select filename and location
  endEdits();
  QString filename = QFileDialog::getSaveFileName();
  if ( filename.isEmpty() ) return false;

  // open the file and check we can write to it
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) )
  {
    ui->statusBar->showMessage( QString("Failed to write to '%1'").arg(filename) );
    return false;
  }

  // call slotTabChange to ensure plan is up to date with 'Properties' tab widgets
  slotTabChange(0);

  // open an xml stream writer and write simulation data
  QXmlStreamWriter  stream( &file );
  QString           who  = getenv("USERNAME");
  QDateTime         when = QDateTime::currentDateTime();
  stream.setAutoFormatting( true );
  stream.writeStartDocument();
  stream.writeStartElement( "qplanner" );
  stream.writeAttribute( "version", "2013-08" );
  stream.writeAttribute( "user", who );
  stream.writeAttribute( "when", when.toString(Qt::ISODate) );
  plan->saveToStream( &stream );
  stream.writeEndDocument();

  // close the file and display useful message
  file.close();
  ui->statusBar->showMessage( QString("Plan saved to '%1'").arg(filename) );

  // update plan properties
  plan->setFileInfo( filename, when, who );
  slotUpdatePropertiesWidgets();
  return true;
}

/***************************************** slotFilePrint *****************************************/

void MainWindow::slotFilePrint()
{
  // slot for file saveAs plan action
  endEdits();
  qDebug("MainWindow::slotFilePrint() - TODO !!!!");
}

/************************************** slotFilePrintPreview *************************************/

void MainWindow::slotFilePrintPreview()
{
  // slot for file saveAs plan action
  endEdits();
  qDebug("MainWindow::slotFilePrintPreview() - TODO !!!!");
}

/****************************************** slotFileExit *****************************************/

void MainWindow::slotFileExit()
{
  // slot for file saveAs plan action
  endEdits();
  qDebug("MainWindow::slotFileExit() - TODO !!!!");
}

/*************************************** slotAboutQPlanner ***************************************/

void MainWindow::slotAboutQPlanner()
{
  // slot for file saveAs plan action
  endEdits();
  qDebug("MainWindow::slotAboutQPlanner() - TODO !!!!");
}

/**************************************** slotSchedulePlan ***************************************/

void MainWindow::slotSchedulePlan()
{
  // get plan to reschedule all the tasks
  endEdits();
  plan->tasks()->schedule();
}

/*************************************** slotStretchTasks ****************************************/

void MainWindow::slotStretchTasks( bool checked )
{
  // if stretch tasks flag is changed, trigger redraw of gantt
  endEdits();
  if ( plan->stretchTasks != checked )
  {
    plan->stretchTasks = checked;
    ui->ganttWidget->update();
  }
}

/*************************************** slotViewUndoStack ***************************************/

void MainWindow::slotUndoStackView( bool checked )
{
  // show undo stack view window if checked, otherwise hide
  endEdits();
  if ( checked )
  {
    if ( m_undoview == nullptr )
    {
      m_undoview = new QUndoView( plan->undostack() );
      m_undoview->setWindowTitle( "Undo stack" );
      m_undoview->setAttribute( Qt::WA_QuitOnClose, false );
      m_undoview->setAttribute( Qt::WA_DeleteOnClose, true );
      connect( m_undoview, SIGNAL(destroyed()), this, SLOT(slotUndoStackViewDestroyed()),
               Qt::UniqueConnection );
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
  m_undoview = nullptr;
  ui->actionUndoStackView->setChecked( false );
}

/***************************************** slotTabChange *****************************************/

void MainWindow::slotTabChange( int index )
{
  // check if switched to tasks/gantt tab, enable its menu & actions, otherwise hide them
  if ( index == ui->mainTabWidget->indexOf(ui->tasksGanttTab) )
  {
    ui->menuTask->menuAction()->setVisible( true );
    ui->actionIndent->setVisible( true );
    ui->actionOutdent->setVisible( true );
  }
  else
  {
    ui->menuTask->menuAction()->setVisible( false );
    ui->actionIndent->setVisible( false );
    ui->actionOutdent->setVisible( false );
  }

  // check if we need to update plan from 'Properties' tab widgets
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

  // ensure 'Properties' tab widgets reflects plan
  slotUpdatePropertiesWidgets();
}

/************************************ updatePropertiesWidgets ************************************/

void MainWindow::slotUpdatePropertiesWidgets()
{
  // ensure 'Properties' tab widgets are up-to-date with what is in plan
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
