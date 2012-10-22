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
#include "plan.h"

#include <QApplication>

/*************************************************************************************************/
// QPlanner by Richard Crook 2012
// Aims to be a project planner similar to M$Project with table entry of tasks & gantt chart
// Also aims to have automatic resource leveling and scheduling based on task priority
// Also aims to have resource levels variable within single task
// Also aims to have gantt chart task bar thickness showing this variable resource usage
// Based on work I started as early as 2005
// IDEA - Resourcing tasks can use special pseudo-resource 'All' & Organisations/Groups/Types
// Progress 2012-10-10 started again using Qt 4.8.0 using QtCreator 2.5.83
/*************************************************************************************************/

Plan*  plan;

int main( int argc, char *argv[] )
{
  // control and provides info to all Qt applications
  QApplication app( argc, argv );

  // create complete data model for the plan
  plan = new Plan();
  plan->initialise();

  // create application main window & enter main event loop
  MainWindow window;
  window.show();
  return app.exec();
}
