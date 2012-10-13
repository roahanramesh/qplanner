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

extern Plan*  plan;

/*************************************************************************************************/
/********************* Main application window showing tabbed main screens ***********************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ), ui( new Ui::MainWindow )
{
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

  // column widths for tables views
  ui->daysView->horizontalHeader()->setDefaultSectionSize( 70 );
  plan->tasks()->setColumnWidths( ui->tasksView );
  plan->resources()->setColumnWidths( ui->resourcesView );
  plan->days()->setColumnWidths( ui->daysView );
}

/****************************************** destructor *******************************************/

MainWindow::~MainWindow()
{
  delete ui;
}
