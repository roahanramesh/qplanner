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
#include "plan.h"
#include "tasksdelegate.h"
#include "resourcesdelegate.h"
#include "tasksmodel.h"
#include "resourcesmodel.h"
#include "daysmodel.h"

/*************************************************************************************************/
/***************************** Tabbed widget containing main screens *****************************/
/*************************************************************************************************/

/****************************************** constructor ******************************************/

MainTabWidget::MainTabWidget( QWidget *parent ) : QTabWidget( parent ), ui( new Ui::MainTabWidget )
{
  // setup palette & ui for main tab widget
  QPalette  pal = palette();
  pal.setBrush( QPalette::Inactive, QPalette::Highlight, QColor("#E0E0E0") );
  setPalette( pal );
  ui->setupUi( this );

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
}

/****************************************** destructor *******************************************/

MainTabWidget::~MainTabWidget()
{
  delete ui;
}
