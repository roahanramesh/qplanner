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

#include <QUndoView>

extern Plan*  plan;

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
  // initialise private variables
  m_undoview = 0;

  // setup ui for main window
  ui->setupUi( this );
  resize( 600, 300 );

  // set models for table views
  ui->tasksView->setModel( (QAbstractItemModel*)plan->tasks() );
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
  ui->daysView->horizontalHeader()->setDefaultSectionSize( 70 );
  ui->calendarsView->horizontalHeader()->setDefaultSectionSize( 150 );
  plan->tasks()->setColumnWidths( ui->tasksView );
  plan->resources()->setColumnWidths( ui->resourcesView );
  plan->days()->setColumnWidths( ui->daysView );

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
  connect( ui->mainTabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabChange(int)) );
}

/*************************************** slotViewUndoStack ***************************************/

void MainWindow::slotUndoStackView( bool checked )
{
  // show undo stack view window if checked, otherwise hide
  if ( checked )
  {
    if ( m_undoview == 0 )
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
  // if changing away from 'Properties' tab then ensure plan is kept up-to-date
  if ( true )
  {
    // TODO !!!!!!
    qDebug("MainWindow::slotTabChange - moving from 'Properties' index=%i",index);


    // return;
  }

  // otherwise ensure 'Properties' tab widgets are up-to-date
  ui->title->setText( plan->title() );
  ui->title->setCursorPosition( 0 );

  ui->planStart->setDateTime( plan->start() );
  ui->planStart->setToolTip( plan->start().toString(ui->dateTimeFormat->text()) );

  ui->planEnd->setText( plan->end().toString("dd/MM/yyyy hh:mm:ss") );
  ui->planEnd->setCursorPosition( 0 );
  ui->planEnd->setToolTip( plan->end().toString(ui->dateTimeFormat->text()) );

  ui->defaultCal->clear();
  ui->defaultCal->addItems( plan->calendars()->namesList() );
  ui->defaultCal->setCurrentIndex( plan->default_cal() );

  ui->dateTimeFormat->setText( plan->datetime_format() );
  ui->dateTimeFormat->setCursorPosition( 0 );
  ui->dateTimeFormat->setToolTip( QDateTime::currentDateTime().toString(ui->dateTimeFormat->text()));

  ui->fileName->setText( plan->filename() );
  ui->fileName->setCursorPosition( 0 );

  ui->fileLocation->setText( plan->file_location() );
  ui->fileLocation->setCursorPosition( 0 );
  ui->fileLocation->setToolTip( plan->file_location() );

  ui->savedBy->setText( plan->saved_by() );
  ui->savedBy->setCursorPosition( 0 );

  ui->savedWhen->setText( plan->saved_when().toString("dd/MM/yyyy hh:mm:ss") );
  ui->savedWhen->setCursorPosition( 0 );
  ui->savedWhen->setToolTip( plan->saved_when().toString(ui->dateTimeFormat->text()) );

  ui->numTasks->setText( QString(": %1").arg(plan->numTasks()) );
  ui->numResources->setText( QString(": %1").arg(plan->numResources()) );
  ui->numCalendars->setText( QString(": %1").arg(plan->numCalendars()) );
  ui->numDays->setText( QString(": %1").arg(plan->numDays()) );
}
